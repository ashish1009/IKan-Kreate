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

  // Core data
  appSpec.title = "Kreator";
  appSpec.coreAssetPath = "../../../IKan/Assets";
  
  // Window Specification
  appSpec.windowSpecificaion.title = "Kreator";
  appSpec.windowSpecificaion.width = 2500;
  appSpec.windowSpecificaion.height = 1000;
  appSpec.windowSpecificaion.hideTitleBar = false;
  appSpec.windowSpecificaion.isFullScreen = false;
  
  // Window Controller
  appSpec.resizable = true;
  appSpec.startMaximized = true;

  // Return the Application
  return Application::CreateApplication<Application>(appSpec);
}
