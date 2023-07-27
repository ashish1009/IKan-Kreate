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
  
  applicationSpec.name = "Kreator";
  return IKan::CreateScope<IKan::Application>(applicationSpec);
}
