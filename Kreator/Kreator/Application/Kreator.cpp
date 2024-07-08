//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 06/07/24.
//

#include "Kreator.hpp"

Scope<Application> CreateApplication(int argc, const char * argv[])
{
  // Create application specification ------------------------------
  ApplicationSpecification applicationSpec;
  applicationSpec.name = "Kreator";
  applicationSpec.rendererType = RendererType::OpenGL;

  // Window Specification Data
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.hideTitleBar = false;
  applicationSpec.windowSpecification.resizable = true;
  applicationSpec.windowSpecification.startMaximized = false;
#ifdef DEBUG
  applicationSpec.windowSpecification.isFullScreen = false;
#else
  applicationSpec.windowSpecification.isFullScreen = true;
#endif

  return Application::CreateApplication<Application>(applicationSpec);
}
