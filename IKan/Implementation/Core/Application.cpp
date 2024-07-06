//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#include "Application.hpp"

namespace IKan
{
  Application::Application(const ApplicationSpecification& appSpec)
  {
  }
  
  Application::~Application()
  {
  }
  
  Application& Application::Get()
  {
    IK_ASSERT(s_instance, "Application instance not created yet !!");
    return *s_instance;
  }
} // namespace IKan
