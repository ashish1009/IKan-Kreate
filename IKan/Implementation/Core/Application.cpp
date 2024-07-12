//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#include "Application.hpp"

#include "Renderer/RendererStats.hpp"

/// IKan Engine Version
static const std::string IKanVersion = "2.0";

namespace IKan
{
  Application::Application(const ApplicationSpecification& appSpec)
  : m_specification(appSpec)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "Application instance already created");
    s_instance = this;
    
    IK_LOG_INFO(LogModule::Application, "Initializing Core application data. Version : {0}", IKanVersion);
    
    // Set the renrerer Type
    Renderer::SetCurrentRendererAPI(m_specification.rendererType);

    // Create the window
    m_window = WindowFactory::Create(m_specification.windowSpecification);
    
    // Set the application callback to window
    m_window->SetEventFunction(IK_BIND_EVENT_FN(Application::HandleEvents));
    
    // Start game loop if window is created
    if (m_window)
    {
      m_isRunning = true;
    }
    
    // Initialize the ImGui Layer
    m_imguiLayer = CreateRef<UI::ImGuiLayer>(m_window.get());
    m_layers.PushOverlay(m_imguiLayer.get());
    m_imguiLayer->SetIniFilePath(m_specification.iniFilePath);

    // Initialize the Renderer
    Renderer::Initialize();
    
    // Execute all commands from queue
    Renderer::WaitAndRender();
  }
  
  Application::~Application()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Application, "Destroying Core application data");

    m_window.reset();
    Renderer::Shutdown();
    m_layers.PopOverlay(m_imguiLayer.get());
    
    s_instance = nullptr;
  }
  
  void Application::Run()
  {
    // Flush the pending task before game loop starts
    FlushBeforeGameLoop();
    
    // Separator in profiler logs before Game Loop
    {
      IK_NAMED_PROFILE("-------------")
    }
    
    // Game Loop
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    
    while (m_isRunning)
    {
      IK_PERFORMANCE("Application::Run");
      
      if (!m_minimized)
      {
        // Updating all the attached layer
        {
          IK_PERFORMANCE("Application::LayersUpdate");
          for (Layer* layer : m_layers)
          {
            layer->OnUpdate(m_timeStep);
          }          
        }
        
        // Cliient virtual override update function
        {
          IK_PERFORMANCE("Application::ClientUpdate");
          OnUpdate(m_timeStep);
        }

        // Render the Gui on Renderer thread
        Renderer::Submit([this](){ RenderImGui(); });
        Renderer::Submit([]() { PerformanceProfiler::Get().Clear(); });
        Renderer::Submit([]() { RendererStatistics::Get().ResetEachFrame(); });

        // Update the window swap buffers
        m_window->Update();
        
        // Store the frame time difference
        m_timeStep = m_window->GetTimestep();

        // Execute all commands from queue before game loop
        Renderer::WaitAndRender();
      }
    }
    IK_LOG_WARN("", "--------------------------------------------------------------------------");
    IK_LOG_WARN("", "                           Ending Game Loop                               ");
    IK_LOG_WARN("", "--------------------------------------------------------------------------");

    // Separator in profiler logs after Game Loop
    {
      IK_NAMED_PROFILE("-------------")
    }
    
    // Flush the pending task after game loop ends
    FlushAfterGameLoop();
  }

  void Application::Close()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Application, "Closing the Application");
    m_isRunning = false;
  }

  void Application::PushLayer(Layer* layer)
  {
    m_layers.PushLayer(layer);
  }
  
  void Application::PopLayer(Layer* layer)
  {
    m_layers.PopLayer(layer);
  }

  const std::string& Application::GetVersion()
  {
    return IKanVersion;
  }
  
  Application& Application::Get()
  {
    IK_ASSERT(s_instance, "Application instance not created yet !!");
    return *s_instance;
  }
  
  bool Application::IsRunning() const
  {
    return m_isRunning;
  }
  bool Application::IsMinimized() const
  {
    return m_minimized;
  }
  TimeStep Application::GetTimestep() const
  {
    return m_timeStep;
  }
  void* Application::GetNativeWindow() const
  {
    return m_window->GetNativeWindow();
  }
  
  Window* Application::GetWindow() const
  {
    return m_window.get();
  }
  UI::ImGuiLayer& Application::GetImGuiLayer() const
  {
    IK_ASSERT(m_imguiLayer, "ImGui Layer is not created yet !!");
    return *m_imguiLayer.get();
  }

  void Application::FlushBeforeGameLoop()
  {
    IK_PROFILE();
    // Initialize the client side application
    OnInit();
    
    // Execute all commands from queue before game loop
    Renderer::WaitAndRender();
  }
  
  void Application::FlushAfterGameLoop()
  {
    IK_PROFILE();
    
    // Shutdown the client side application
    OnShutdown();
    
    // Execute all commands from queue after game loop
    Renderer::WaitAndRender();
  }
  
  void Application::HandleEvents(Event &event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
    dispatcher.Dispatch<WindowMinimizeEvent>(IK_BIND_EVENT_FN(Application::WindowMinimize));
    dispatcher.Dispatch<WindowMaximizeEvent>(IK_BIND_EVENT_FN(Application::WindowMaximize));
    
    // Event Handler for all layers
    for (Layer* layer : m_layers)
    {
      layer->OnEvent(event);
    }
    
    // Client side event handler funciton
    OnEvent(event);
  }
  
  bool Application::WindowClose([[maybe_unused]] WindowCloseEvent& windowCloseEvent)
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Application, "Closing the Window from application");
    Close();
    return false;
  }
  
  bool Application::WindowMinimize(WindowMinimizeEvent& windowMinimizeEvent)
  {
    m_minimized = windowMinimizeEvent.IsMinimized();
    return false;
  }
  
  bool Application::WindowMaximize([[maybe_unused]] WindowMaximizeEvent &windowResizeEvent)
  {
    m_minimized = false;
    return false;
  }
  
  void Application::RenderImGui()
  {
    IK_ASSERT(m_imguiLayer, "ImGui Layer is not created yet !!");
    
    m_imguiLayer->Begin();
    
    // Render Imgui for all layers
    for (Layer* layer : m_layers)
    {
      layer->OnImGuiRender();
    }
    
    m_imguiLayer->End();
  }
} // namespace IKan
