//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>
#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"
#include "Widget.hpp"
#include "ApplicationSettings.hpp"
#include "SceneHierarchyPanel.hpp"

namespace Kreator
{  
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
    
  private:
    Ref<Layer> m_rendereLayer;
  };
} // namespace Kreator

IKan::Scope<IKan::Application> IKan::CreateApplication(const std::filesystem::path& startupProject)
{
  // Set the client resource path
  std::filesystem::path clientResourcePath = "../../../Kreator/Resources";

  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
  applicationSpec.name = "Kreator";
  applicationSpec.renderingApi = Renderer::Api::OpenGl;
  applicationSpec.engineResourcesPath = "../../../IKan/Assets";
  applicationSpec.iniPath = clientResourcePath / "Kreator.ini";

  // Window Specification
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.isFullscreen = false;
  applicationSpec.windowSpecification.hideTitleBar = true;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;

  return IKan::CreateScope<Kreator::KreatorApp>(applicationSpec);
}
