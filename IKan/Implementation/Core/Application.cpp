//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#include "Application.hpp"

/// IKan Engine Version
static const std::string IKanVersion = "1.1";

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
    
    // Initialize the Renderer
    Renderer::Initialize();
  }
  
  Application::~Application()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Application, "Destroying Core application data");

    m_window.reset();
    Renderer::Shutdown();
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
        
        // Update the window swap buffers
        m_window->Update();
        
        // Store the frame time difference
        m_timeStep = m_window->GetTimestep();

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

  void Application::FlushBeforeGameLoop()
  {
    IK_PROFILE();
    // Initialize the client side application
    OnInit();
  }
  
  void Application::FlushAfterGameLoop()
  {
    IK_PROFILE();
    // Shutdown the client side application
    OnShutdown();
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
  
  bool Application::WindowMaximize(WindowMaximizeEvent &windowResizeEvent)
  {
    m_minimized = false;
    return false;
  }
} // namespace IKan
