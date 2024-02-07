//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include "Application.hpp"

namespace IKan
{
  Application::Application(const ApplicationSpecification& appSpec)
  {
    IK_ASSERT(!s_instance, "Application instance already created");
    s_instance = this;
    
    IK_LOG_INFO(LogModule::Application, "Initializing Core application data");
  }
  Application::~Application()
  {
    IK_LOG_INFO(LogModule::Application, "Destroying Core application data");
  }
} // namespace IKan
