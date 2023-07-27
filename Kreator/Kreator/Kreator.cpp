//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>

IKan::Scope<IKan::Application> IKan::CreateApplication(int argc, const char** argv)
{
  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
 
  // Window Specification
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.isFullscreen = false;
  applicationSpec.windowSpecification.hideTitleBar = false;

  applicationSpec.name = "Kreator";
  return IKan::CreateScope<IKan::Application>(applicationSpec);
}
