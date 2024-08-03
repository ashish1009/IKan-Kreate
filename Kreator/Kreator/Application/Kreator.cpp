//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 06/07/24.
//

#include "Kreator.hpp"
#include "Layers/RendererLayer.hpp"
#include "Editor/UserPreference.hpp"
#include "Editor/FolderExplorer.hpp"
#include "Panels/SceneHierarchyPanel.hpp"

namespace Kreator
{
  /// IKan Engine Version
  static const std::string KreatorVersion = "3.1";
  
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec, const KreatorDirectories& kreatorDirectories)
  : Application(appSpec), m_kreatorDirectories(kreatorDirectories)
  {
    IK_PROFILE();
    IK_LOG_INFO("KreatorApp", "Creating 'Kreator' Application. Version : {0}", KreatorVersion);
  }
  
  KreatorApp::~KreatorApp()
  {
    IK_PROFILE();
    IK_LOG_WARN("KreatorApp", "Destroying 'Kreator' Application");
  }

  void KreatorApp::OnInit()
  {
    IK_PROFILE();

    // Check Client directories ---------------------------------------------------------------------------------------
    [[maybe_unused]] bool exist {std::filesystem::exists(m_kreatorDirectories.clientResourcePath)};
    [[maybe_unused]] bool tempalateProj2D {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "TemplateProject2D")};
    [[maybe_unused]] bool tempalateProj3D {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "TemplateProject3D")};
    [[maybe_unused]] bool fonts {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "Fonts")};
    [[maybe_unused]] bool textures {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "Textures")};

    IK_ASSERT(exist and tempalateProj2D and tempalateProj3D and fonts and textures, "Invalid Client Directory");

    IK_LOG_INFO("KreatorApp", "Initializing the 'Kreator' Application");
    IK_LOG_INFO("Kreator App", "  Systerm User Path       : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_kreatorDirectories.systemUserPath).string());
    IK_LOG_INFO("Kreator App", "  Kreator Path            : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_kreatorDirectories.kreatorPath).string());
    IK_LOG_INFO("Kreator App", "  Kreator Resources Path  : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_kreatorDirectories.clientResourcePath).string());

    // Create Persistance Directory -----------------------------------------------------------------------------------
    std::filesystem::path persistenceStoragePath {m_kreatorDirectories.clientResourcePath / "UserData"};
    if (!std::filesystem::exists(persistenceStoragePath))
    {
      std::filesystem::create_directory(persistenceStoragePath);
    }
    IK_LOG_INFO("Kreator App", "  User Data Path          : {0}", IKan::Utils::FileSystem::IKanAbsolute(persistenceStoragePath).string());

    // User Preferences --------------------------------------------------------------------------
    Ref<UserPreferences> userPreferences {CreateRef<UserPreferences>()};
    UserPreferencesSerializer serializer(userPreferences);
    std::filesystem::path userPreferenceFile {persistenceStoragePath / "UserPreferences.yaml"};
    if (!std::filesystem::exists(userPreferenceFile))
    {
      serializer.Serialize(userPreferenceFile);
    }
    else
    {
      serializer.Deserialize(userPreferenceFile);
    }

    // Creating Renderer layer and push in application stack
    m_rendererLayer = CreateScope<RendererLayer>(m_kreatorDirectories, userPreferences);
    PushLayer(m_rendererLayer.get());
    
    // Initialize the Kreator Modules -------------------------------------------------------------
    // Should get initialized after layer initialize
    FolderExplorer::Initialize();
    UI::Widgets::Initialize();
    SceneHierarchyPanel::Initialize();
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("KreatorApp", "Shutting Down the 'Kreator' Application");

    // Shutdown the Kreator Modules -------------------------------------------------------------
    FolderExplorer::Shutdown();
    UI::Widgets::Shutdown();
    SceneHierarchyPanel::Shutdown();

    // Removing Renderer layer from application stack and destroying its instance
    PopLayer(m_rendererLayer.get());
    m_rendererLayer.reset();
  }
  
  void KreatorApp::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("KreatorApp::OnUpdate");
  }
  
  void KreatorApp::OnImGuiRender()
  {
    IK_PERFORMANCE("KreatorApp::OnImGuiRender");
  }
  
  const std::string& KreatorApp::GetVersion()
  {
    return KreatorVersion;
  }
} // namespace Kreator

Scope<Application> CreateApplication([[maybe_unused]] int argc, [[maybe_unused]] const char * argv[])
{
  // Create application specification ------------------------------
  ApplicationSpecification applicationSpec;
  applicationSpec.name = "Kreator";
  applicationSpec.rendererType = RendererType::OpenGL;

  // Window Specification Data
  applicationSpec.windowSpecification.title = "Kreator";
  applicationSpec.windowSpecification.width = 1600;
  applicationSpec.windowSpecification.height = 900;
  applicationSpec.windowSpecification.hideTitleBar = true;
  applicationSpec.windowSpecification.isFullScreen = false;

  applicationSpec.resizable = true;
  applicationSpec.startMaximized = true;

  // Ini file
  applicationSpec.iniFilePath = "../../../Kreator/Kreator.ini";

  // Editor Data --------------------------------------------------------------------------
  // TODO: Kreator: Extract from arguments
  Kreator::KreatorDirectories kreatorDirectories;
  kreatorDirectories.systemUserPath = "/Users/ashish./iKan_storage";
  kreatorDirectories.kreatorPath = "/Users/ashish./iKan_storage/Github/Product/IKan-Kreate";
  kreatorDirectories.clientResourcePath = "/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/Kreator/Resources";

  return Application::CreateApplication<Kreator::KreatorApp>(applicationSpec, kreatorDirectories);
}
