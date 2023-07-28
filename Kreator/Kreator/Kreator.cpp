//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>

IKan::Scope<IKan::Application> IKan::CreateApplication(const ApplicationData& appData)
{
  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
 
  applicationSpec.name = "Kreator";
  applicationSpec.engineInstallPath = appData.engineInstallPath;
  applicationSpec.renderingApi = IKan::Renderer::Api::OpenGl;

  // Window Specification
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.isFullscreen = false;
  applicationSpec.windowSpecification.hideTitleBar = false;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;

  return IKan::CreateScope<IKan::Application>(applicationSpec);
}
