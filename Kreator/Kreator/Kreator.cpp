//
//  main.cpp
//  Kreator
//
//  Created by Ashish . on 27/07/23.
//

#include <IKanEntryPoint.h>
#include "RendererLayer.hpp"

namespace Kreator
{
  using namespace IKan;
  
  class KreatorApp : public Application
  {
  public:
    KreatorApp(const Application::Specification& appSpec, const std::string& clientDirectory, const std::string& startProject)
    : Application(appSpec), m_userPreference(CreateRef<UserPreferences>()), m_projectPath(startProject)
    {
      IK_LOG_TRACE("Kreator App", "Creating Kreator Application");
      
      // Check Client Path is valid
      IK_LOG_TRACE("Kreator App", "Setting Client Directory {0}", Utils::FileSystem::Absolute(clientDirectory));
      bool exist = Utils::FileSystem::Exists(clientDirectory);
      bool kreator = Utils::FileSystem::Exists(clientDirectory + "/Kreator");
      bool resources = Utils::FileSystem::Exists(clientDirectory + "/Resources");
      
      IK_ASSERT(exist and kreator and resources, "Invalid Client Directory");
      m_clientDirectory = clientDirectory;
   }
    
    ~KreatorApp()
    {
      IK_LOG_WARN("Kreator App", "Destroying Kreator Application");
    }
    
    void OnInit() override
    {
      // Create Persistance Directory
      m_persistenceStoragePath = m_clientDirectory / "PrsistenceStorage";
      if (!Utils::FileSystem::Exists(m_persistenceStoragePath))
      {
        Utils::FileSystem::CreateDirectory(m_persistenceStoragePath);
      }

      // Create projects Dir
      auto projectDir = m_clientDirectory / "Projects";
      if (!Utils::FileSystem::Exists(projectDir))
      {
        Utils::FileSystem::CreateDirectory(projectDir);
      }

      // User Preferences
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
      } // User Preferences
      
      // Project
      {
        // Update the project Path
        if (m_projectPath.empty())
        {
          if (m_userPreference->startupProject.empty())
          {
#if 0
            // If There is no startup project and no project is given by argument
            // Create Projects Directory
            auto sandboxProjectDir = projectDir / "SandboxProject";
            m_projectPath = sandboxProjectDir / "Sandbox.ikproj";
            m_userPreference->startupProject = m_projectPath;
#endif
          }
          else
          {
            IK_ASSERT(false);
          }
        }
        else
        {
          IK_ASSERT(false);
        }
      }
      
      IK_LOG_TRACE("Kreator App", "  Application Path           : {0}", m_clientDirectory.string().c_str());
      IK_LOG_TRACE("Kreator App", "  Persistance storage Path   : {0}", m_persistenceStoragePath.string().c_str());
      IK_LOG_TRACE("Kreator App", "  Startup Project            : {0}", m_projectPath.string().c_str());

      // Create the Rendere Layer
      m_rendereLayer = CreateRef<RendererLayer>(m_userPreference, m_clientDirectory);
      
      // Push the renderer Layer in Application
      PushLayer(m_rendereLayer);
    }
    
    void OnShutdown() override
    {
      PopLayer(m_rendereLayer);
      m_rendereLayer.reset();
    }
    
  private:
    Ref<Layer> m_rendereLayer;
    std::filesystem::path m_clientDirectory;
    std::filesystem::path m_persistenceStoragePath;
    std::filesystem::path m_projectPath;
    Ref<UserPreferences> m_userPreference;
  };
} // namespace Kreator

IKan::Scope<IKan::Application> IKan::CreateApplication(const ApplicationData& appData)
{
  // Set up all the applicaiton specification
  IKan::Application::Specification applicationSpec;
 
  applicationSpec.name = "Kreator";
  applicationSpec.engineInstallPath = appData.engineInstallPath;
  applicationSpec.iniPath = appData.clientDirectoryPath + "/Kreator.ini";
  applicationSpec.renderingApi = IKan::Renderer::Api::OpenGl;

  // Window Specification
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.isFullscreen = false;
  applicationSpec.windowSpecification.hideTitleBar = true;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;

  return IKan::CreateScope<Kreator::KreatorApp>(applicationSpec, appData.clientDirectoryPath, appData.startupProject);
}
