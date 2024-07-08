//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#include "Application.hpp"

/// IKan Engine Version
static const std::string IKanVersion = "1.0";

namespace IKan
{
  Application::Application(const ApplicationSpecification& appSpec)
  : m_specification(appSpec)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "Application instance already created");
    s_instance = this;
    
    IK_LOG_INFO(LogModule::Application, "Initializing Core application data. Version : {0}", IKanVersion);
  }
  
  Application::~Application()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Application, "Destroying Core application data");

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
} // namespace IKan
