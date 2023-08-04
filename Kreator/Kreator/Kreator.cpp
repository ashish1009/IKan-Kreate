//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>
#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"

namespace Kreator
{
  using namespace IKan;
  
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const Application::Specification& appSpec, const std::string& clientResourcePath, const std::string& startProject)
    : Application(appSpec), m_userPreference(CreateRef<UserPreferences>()), m_projectPath(startProject)
    {
      IK_LOG_TRACE("Kreator App", "Creating Kreator Application");
      
      // Check Client Path is valid
      IK_LOG_TRACE("Kreator App", "Setting Client Resources Directory {0}", Utils::FileSystem::IKanAbsolute(clientResourcePath));
      bool exist = Utils::FileSystem::Exists(clientResourcePath);
      bool tempalateProj = Utils::FileSystem::Exists(clientResourcePath + "/TemplateProject");
      bool fonts = Utils::FileSystem::Exists(clientResourcePath + "/Fonts");
      bool textures = Utils::FileSystem::Exists(clientResourcePath + "/Textures");
      IK_ASSERT(exist and tempalateProj and fonts and textures, "Invalid Client Directory");
      m_clientResourcePath = clientResourcePath;
   }
    
    ~KreatorApp()
    {
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {
      // Create Persistance Directory ---------------------------------------------------------------
#ifdef DEBUG
      m_persistenceStoragePath = m_clientResourcePath / "../PersistenceStorage";
#else
      m_persistenceStoragePath = "PersistenceStorage";
#endif
      if (!Utils::FileSystem::Exists(m_persistenceStoragePath))
      {
        Utils::FileSystem::CreateDirectory(m_persistenceStoragePath);
      }

      // Create projects Dir
      std::filesystem::path projectDir = "";
#ifdef DEBUG
      projectDir = m_clientResourcePath / "../Projects";
#else
      projectDir = "Projects";
#endif
      if (!Utils::FileSystem::Exists(projectDir))
      {
        Utils::FileSystem::CreateDirectory(projectDir);
      }

      // User Preferences --------------------------------------------------------------------------
      {
        UserPreferencesSerializer serializer(m_userPreference);
        std::filesystem::path userPreferenceFile = m_persistenceStoragePath / "UserPreferences.yaml";
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
        if (m_projectPath.empty())
        {
          if (m_userPreference->startupProject.empty())
          {
            // DO Nothing it will popup the welcome screen
          }
          else
          {
            // DO Nothing it will popup the welcome screen
          }
        }
        else
        {
          if (Utils::FileSystem::Exists(m_projectPath))
          {
            m_userPreference->startupProject = m_projectPath;
            serializer.Serialize(userPreferenceFile);
          }
        }
      }
      
      IK_LOG_TRACE("Kreator App", "  Kreator Resources Path     : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_clientResourcePath));
      IK_LOG_TRACE("Kreator App", "  Persistance storage Path   : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_persistenceStoragePath));
      IK_LOG_TRACE("Kreator App", "  Startup Project            : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_userPreference->startupProject));

      // Create and Push the Rendere Layer --------------------------------------------------------
      m_rendereLayer = CreateRef<RendererLayer>(m_userPreference, m_clientResourcePath);
      PushLayer(m_rendereLayer);
      
      // Initialize the Kreator Modules -------------------------------------------------------------
      // Should get initialized after layer initialize
      FolderExplorer::Initialize();
    }
    
    void OnShutdown() override
    {
      // Shutdown the Kreator Modules -------------------------------------------------------------
      FolderExplorer::Shutdown();
      
      // Destroy and Pop the Rendere Layer --------------------------------------------------------
      PopLayer(m_rendereLayer);
      m_rendereLayer.reset();
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
  applicationSpec.windowSpecification.hideTitleBar = true;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;
  

  return IKan::CreateScope<Kreator::KreatorApp>(applicationSpec, clientResourcePath, startupProject);
}
