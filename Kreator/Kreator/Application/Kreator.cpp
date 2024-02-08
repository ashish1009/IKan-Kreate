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

  // Core Data
  appSpec.title = "Kreator";
  appSpec.coreAssetPath = "../../../IKan/Assets";
  
  // Renderer Data
  appSpec.rendererType = RendererType::OpenGL;
  
  // Window Specification Data
  appSpec.windowSpecificaion.title = "Kreator";
  appSpec.windowSpecificaion.width = 2500;
  appSpec.windowSpecificaion.height = 1000;
  appSpec.windowSpecificaion.hideTitleBar = false;
  appSpec.windowSpecificaion.isFullScreen = false;
  
  // Window Controller Data
  appSpec.resizable = true;
  appSpec.startMaximized = true;

  // Return the Application
  return Application::CreateApplication<Application>(appSpec);
}
