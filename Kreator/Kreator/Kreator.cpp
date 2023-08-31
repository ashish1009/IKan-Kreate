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
    KreatorApp(const Application::Specification& appSpec, const std::string& clientResourcePath, const std::string& startProject)
    : Application(appSpec), m_userPreference(CreateRef<UserPreferences>()), m_projectPath(startProject)
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
    std::filesystem::path m_clientResourcePath;
    std::filesystem::path m_persistenceStoragePath;
    std::filesystem::path m_projectPath;
    Ref<UserPreferences> m_userPreference;
  };
} // namespace Kreator

IKan::Scope<IKan::Application> IKan::CreateApplication(const std::filesystem::path& startupProject)
{
  std::filesystem::path clientResourcePath = "";
#ifdef DEBUG
  clientResourcePath = "../../../Kreator/Resources";
#else
  clientResourcePath = "Resources/Kreator";
#endif

  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
 
  applicationSpec.name = "Kreator";

#ifdef DEBUG
  applicationSpec.engineResourcesPath = "../../../IKan/Assets";
#else
  applicationSpec.engineResourcesPath = "Resources/Core";
#endif
  
  applicationSpec.iniPath = clientResourcePath / "Kreator.ini";
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
