//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Application.hpp"

// TODO: To Move rendere layer later
#include "Shader.hpp"
#include "RendererBuffers.hpp"

namespace IKan
{
  // TODO: To Move rendere layer later
  static Ref<Shader> m_Shader;
  static Ref<IndexBuffer> m_indexBuffer;
  
  Application::Application(const Specification& spec)
  : m_specificaion(spec)
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

    // Initialise the Core Renderer
    Renderer::Initialize();
    
    // TODO: To Move Renderer Layer later
    {
      m_Shader = Shader::Create(CoreAssetPath("Shaders/QuadShader.glsl"));
      
      uint32_t indices[] = {
        0, 1, 2, 2, 3, 0
      };
      
      m_indexBuffer = IndexBuffer::CreateWithSize(indices, sizeof(indices));
    }

    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                     Core Application Initialised                         ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  Application::~Application()
  {
    IK_PROFILE();

    // TODO: To Move Renderer Layer later
    {
      m_indexBuffer.reset();
      m_Shader.reset();
    }

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

      // Update the client application
      OnUpdate(m_timeStep);
      
      // Render the Gui for Application
      ImguiRender();
      
      // Clear Runtime Performance profiler
      PerformanceProfiler::Get()->Clear();
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
    // Rendering Imgui for Client
    OnImguiRender();
  }

} // namespace IKan
