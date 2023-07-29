//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Application.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan
{
  Application::Application(const Specification& spec)
  : m_specificaion(spec), m_layers(CreateScope<LayerStack>())
  {
    IK_PROFILE();

    IK_ASSERT(!s_instance, "Application instance already created");
    // Copy the single instance of application
    s_instance = this;

    IK_LOG_TRACE(LogModule::Application, "Creating Core Application Instance : {0}", m_specificaion.name);
    
    // Create Renderer Instance before any GLFW or Renderer Context
    // NOTE: Creating the Renderer Data Memory in very begining as this will setup the Renderer API to be used to
    //       create any Renderer Implementation
    Renderer::CreateRendererData(m_specificaion.renderingApi);

    // Update the core engine directory path
    CoreDirectory::SetPath(m_specificaion.engineInstallPath);

    // Create the window
    m_window = Window::Create(m_specificaion.windowSpecification);
    
    // Set the application callback to window
    m_window->SetEventFunction(IK_BIND_EVENT_FN(Application::HandleEvents));
    
    // Control Window
    m_window->SetResizable(m_specificaion.resizable);
    if (m_specificaion.startMaximized)
    {
      m_window->Maximize();
    }

    // Initialize the Imgui Layer if GUI is enabled
    m_imguiLayer = CreateRef<UI::ImguiLayer>(m_window);
    m_layers->PushOverlay(m_imguiLayer);
    m_imguiLayer->SetIniFilePath(m_specificaion.iniPath);

    // Initialise the Core Renderer
    Renderer::Initialize();
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                     Core Application Initialised                         ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  Application::~Application()
  {
    IK_PROFILE();

    // Reset the window
    m_window.reset();

    // Shutdown the Renderer
    Renderer::Shutdown();

    IK_LOG_WARN(LogModule::Application, "Destroying Core Application Instance : {0}", m_specificaion.name);
  }

  void Application::Run()
  {
    {
      IKan::ScopedTimer onShutdown("Client Application Initialize()");
      // Initialise the client application
      OnInit();
    }
    // GAME LOOP -----------------------------------------------------------------------------------------------------
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    
    while (m_isRunning)
    {
      // Update the window swap buffers
      m_window->Update();

      // Store the frame time difference
      m_timeStep = m_window->GetTimestep();

      // Updating all the attached layer
      for (auto& layer : *(m_layers.get()))
      {
        layer->OnUpdate(m_timeStep);
      }

      // Update the client application
      OnUpdate(m_timeStep);
      
      // Render the Gui for Application
      ImguiRender();
      
      // Clear Runtime Performance profiler
      PerformanceProfiler::Get()->Clear();
      
      // Reset Statistics each frame
      RendererStatistics::Get().ResetEachFrame();
    }
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                           Ending Game Loop                               ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");

    {
      IKan::ScopedTimer onShutdown("Client Application Shutdown()");
      // Shutdown the client application
      OnShutdown();
    }
  }
  
  void Application::HandleEvents(Event &event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
        
    // Event Handler for all layers
    for (auto& layer : *(m_layers.get()))
    {
      layer->OnEvent(event);
    }
    
    // Client side event handler funciton
    OnEvent(event);    
  }
  
  bool Application::WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event)
  {
    Close();
    return false;
  }
  
  void Application::Close()
  {
    m_isRunning = false;
    IK_LOG_WARN(LogModule::Application, "Closing the Application");
  }
  
  void Application::ImguiRender()
  {
    m_imguiLayer->Begin();
    
    // Updating all the attached layer
    for (auto& layer : *(m_layers.get()))
    {
      layer->OnImguiRender();
    }

    // Rendering Imgui for Client
    OnImguiRender();

    m_imguiLayer->End();
  }
  
  void Application::PushLayer(const Ref<Layer> &layer)
  {
    m_layers->PushLayer(layer);
  }
  
  void Application::PopLayer(const Ref<Layer> &layer)
  {
    m_layers->PopLayer(layer);
  }

  void* Application::GetWindowPtr() const
  {
    return m_window->GetNativeWindow();
  }
  
  Window& Application::GetWindow()
  {
    return *(m_window.get());
  }

  UI::ImguiLayer& Application::GetImGuiLayer() const
  {
    return *m_imguiLayer.get();
  }

  Application& Application::Get()
  {
    return *s_instance;
  }
  
} // namespace IKan
