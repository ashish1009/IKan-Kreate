//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 07/02/24.
//

#include "Kreator.hpp"

Scope<Application> CreateApplication()
{
  IK_PROFILE();
  
  // Core Application Specificaion --------------------------------------------------------
  ApplicationSpecification appSpec;

  // Return the Application
  return Application::CreateApplication<Application>(appSpec);
}
