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
      
      // Check Client Path is valid
      bool exist = Utils::FileSystem::Exists(clientResourcePath);
      bool tempalateProj = Utils::FileSystem::Exists(clientResourcePath + "/TemplateProject");
      bool fonts = Utils::FileSystem::Exists(clientResourcePath + "/Fonts");
      bool textures = Utils::FileSystem::Exists(clientResourcePath + "/Textures");
      IK_ASSERT(exist and tempalateProj and fonts and textures, "Invalid Client Directory");
      IK_LOG_TRACE("Kreator App", "Setting Client Resources Directory {0}", Utils::FileSystem::IKanAbsolute(clientResourcePath));
    }
    
    ~KreatorApp()
    {
      IK_PROFILE();
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {
      IK_PROFILE();
      
      // Create Persistance Directory ---------------------------------------------------------------
#ifdef DEBUG
      std::filesystem::path persistenceStoragePath = m_clientResourcePath / "../PersistenceStorage";
#else
      std::filesystem::path persistenceStoragePath = m_clientResourcePath / "../PersistenceStorage";
#endif
      if (!Utils::FileSystem::Exists(persistenceStoragePath))
      {
        Utils::FileSystem::CreateDirectory(persistenceStoragePath);
      }
      
      // Create projects Directory ------------------------------------------------------------------
#ifdef DEBUG
      std::filesystem::path projectDir = m_clientResourcePath / "../Projects";
#else
      std::filesystem::path projectDir = m_clientResourcePath / "../Projects";
#endif
      if (!Utils::FileSystem::Exists(projectDir))
      {
        Utils::FileSystem::CreateDirectory(projectDir);
      }

      // User Preferences --------------------------------------------------------------------------
      Ref<UserPreferences> userPreference =  CreateRef<UserPreferences>();
      userPreference->theme = UserPreferences::Theme::Blue;
      
      UserPreferencesSerializer serializer(userPreference);
      std::filesystem::path userPreferenceFile = persistenceStoragePath / "UserPreferences.yaml";
      if (!Utils::FileSystem::Exists(userPreferenceFile))
      {
        serializer.Serialize(userPreferenceFile);
      }
      else
      {
        serializer.Deserialize(userPreferenceFile);
      }

      // Project -----------------------------------------------------------------------------------
      // Update the project Path
      if (m_startupProjectPath.empty())
      {
        if (userPreference->startupProject.empty())
        {
          // DO Nothing it will popup the welcome screen
        }
        else
        {
          // DO Nothing it will Open the Project in Renderer Layer
        }
      }
      else
      {
        if (Utils::FileSystem::Exists(m_startupProjectPath))
        {
          userPreference->startupProject = m_startupProjectPath;
          serializer.Serialize(userPreferenceFile);
        }
      }
      
      IK_LOG_TRACE("Kreator App", "  Kreator Resources Path     : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_clientResourcePath));
      IK_LOG_TRACE("Kreator App", "  Persistance storage Path   : {0}", IKan::Utils::FileSystem::IKanAbsolute(persistenceStoragePath));
      if (userPreference->startupProject != "")
      {
        IK_LOG_TRACE("Kreator App", "  Startup Project            : {0}", IKan::Utils::FileSystem::IKanAbsolute(userPreference->startupProject));
      }

      IK_LOG_TRACE("Kreator App", " ++++++++++++           Initializing the Renderer Layer            +++++++++++++++");

      // Create and Push the Rendere Layer --------------------------------------------------------
      m_rendereLayer = CreateRef<RendererLayer>(userPreference, m_clientResourcePath);
      PushLayer(m_rendereLayer);
      
      IK_LOG_TRACE("Kreator App", " ++++++++++++          Initializing the Application Data           +++++++++++++++");
      
      // Initialize the Kreator Modules -------------------------------------------------------------
      // Should get initialized after layer initialize
      FolderExplorer::Initialize();
      Kreator_UI::Widgets::Initialize();
      ApplicationSettingsSerializer::Initialize();
      SceneHierarchyPanel::Initialize();
    }

    void OnShutdown() override
    {
      IK_PROFILE();

      // Shutdown the Kreator Modules -------------------------------------------------------------
      SceneHierarchyPanel::Shutdown();
      Kreator_UI::Widgets::Shutdown();
      FolderExplorer::Shutdown();
      
      // Destroy and Pop the Rendere Layer --------------------------------------------------------
      PopLayer(m_rendereLayer);
      m_rendereLayer.reset();
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
