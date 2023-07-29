//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>

namespace Kreator
{
  using namespace IKan;
  
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const Application::Specification& appSpec)
    : Application(appSpec)
    {
      IK_LOG_TRACE("Kreator App", "Creating Kreator Application");
    }
    
    ~KreatorApp()
    {
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {
 
    }
    
    void OnShutdown() override
    {
      
    }
  };
} // namespace Kreator

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

  return IKan::CreateScope<Kreator::KreatorApp>(applicationSpec);
}
