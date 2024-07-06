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
  
  const std::string& Application::GetVersion()
  {
    return IKanVersion;
  }
  
  Application& Application::Get()
  {
    IK_ASSERT(s_instance, "Application instance not created yet !!");
    return *s_instance;
  }
} // namespace IKan
