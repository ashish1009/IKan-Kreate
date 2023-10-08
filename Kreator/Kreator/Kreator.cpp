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
#include "UserPreference.hpp"
#include "DummyApplication.hpp"

namespace Kreator
{  
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const Application::Specification& appSpec, const std::string& clientResourcePath, const std::string& startupProject)
    : Application(appSpec), m_clientResourcePath(clientResourcePath), m_startupProjectPath(startupProject)
    {
      IK_PROFILE();
      IK_LOG_TRACE("Kreator App", "Creating Kreator Application");
    }
    
    ~KreatorApp()
    {
      IK_PROFILE();
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {
      IK_PROFILE();
    }

    void OnShutdown() override
    {
      IK_PROFILE();
    }
    
  private:
    Ref<Layer> m_rendereLayer;
    std::filesystem::path m_clientResourcePath;
    std::filesystem::path m_startupProjectPath;
  };
} // namespace Kreator

IKan::Scope<IKan::Application> IKan::CreateApplication(const std::filesystem::path& startupProject)
{
  // Set the client resource path
#ifdef DEBUG
  std::filesystem::path clientResourcePath = "../../../Kreator/Resources";
#else
  std::filesystem::path clientResourcePath = "../../../Kreator/Resources";
#endif
  
  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
  applicationSpec.name = "Kreator";
  applicationSpec.renderingApi = Renderer::Api::OpenGl;
#ifdef DEBUG
  applicationSpec.engineResourcesPath = "../../../IKan/Assets";
#else
  applicationSpec.engineResourcesPath = "../../../IKan/Assets";
#endif
  applicationSpec.iniPath = clientResourcePath / "Kreator.ini";

  // Window Specification
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.isFullscreen = false;
  applicationSpec.windowSpecification.hideTitleBar = true;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;
  
  return IKan::CreateScope<Kreator::KreatorApp>(applicationSpec, clientResourcePath, startupProject);
}
