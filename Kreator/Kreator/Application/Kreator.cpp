//
//  Kreator.cpp
//  Kreator
//
//  Created by Ashish . on 07/02/24.
//

#include "Kreator.hpp"
#include "Layer/KreatorLayer.hpp"
#include "Editor/UserPreferences.hpp"
#include "Editor/FolderExplorer.hpp"
#include "Editor/ApplicationSettings.hpp"
#include "Panel/SceneHierarchyPanel.hpp"
#include "UI/ImGuiWidget.hpp"

namespace Kreator
{
  KreatorApp::KreatorApp(const ApplicationSpecification& appSpec, const KreatorDirectories& kreatorDirectories)
  : Application(appSpec), m_kreatorDirectories(kreatorDirectories)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Creating Kreator Application");
    
    // Check Client Path is valid
    bool exist {std::filesystem::exists(m_kreatorDirectories.clientResourcePath)};
    bool tempalateProj {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "TemplateProject")};
    bool fonts {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "Fonts")};
    bool textures {std::filesystem::exists(m_kreatorDirectories.clientResourcePath / "Textures")};
    
    IK_ASSERT(exist and tempalateProj and fonts and textures, "Invalid Client Directory");
  }
  
  KreatorApp::~KreatorApp()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Destroying Kreator Application");
  }
  
  void KreatorApp::OnInit()
  {
    IK_PROFILE();
    // Create Persistance Directory ---------------------------------------------------------------
    std::filesystem::path persistenceStoragePath {m_kreatorDirectories.clientResourcePath / "UserData"};
    if (!std::filesystem::exists(persistenceStoragePath))
    {
      std::filesystem::create_directory(persistenceStoragePath);
    }
    
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

    IK_LOG_INFO("Kreator App", "Initializing the Renderer Application");
    IK_LOG_INFO("Kreator App", "  Kreator Resources Path   : {0}", IKan::Utils::FileSystem::IKanAbsolute(m_kreatorDirectories.clientResourcePath).string());
    IK_LOG_INFO("Kreator App", "  User Data Path           : {0}", IKan::Utils::FileSystem::IKanAbsolute(persistenceStoragePath).string());

    // Create and Push the Rendere Layer --------------------------------------------------------
    m_kreatorLayer = CreateRef<KreatorLayer>(m_kreatorDirectories.clientResourcePath, m_kreatorDirectories.systemUserPath, m_kreatorDirectories.iKanKreatePath, userPreferences);
    PushLayer(m_kreatorLayer);
    
    // Initialize the Kreator Modules -------------------------------------------------------------
    // Should get initialized after layer initialize
    FolderExplorer::Initialize();
    UI::Widgets::Initialize();
    ApplicationSettingsSerializer::Initialize();
    SceneHierarchyPanel::Initialize();
  }
  
  void KreatorApp::OnShutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator App", "Shutting Down the Renderer Application");

    // Shutdown the Kreator Modules -------------------------------------------------------------
    FolderExplorer::Shutdown();
    UI::Widgets::Shutdown();
    SceneHierarchyPanel::Shutdown();
    
    // Destroy and Pop the Rendere Layer --------------------------------------------------------
    PopLayer(m_kreatorLayer);
    m_kreatorLayer.reset();
  }
  
  void KreatorApp::OnUpdate(TimeStep ts)
  {

  }
  
  void KreatorApp::OnImGuiRender()
  {

  }
} // namespace Kreator

Scope<Application> CreateApplication()
{
  IK_PROFILE();
  
  // Core Application Specificaion --------------------------------------------------------
  ApplicationSpecification appSpec;

  // Core Data
  appSpec.title = "Kreator";
  appSpec.coreAssetPath = "../../../IKan/Assets";
  
  // Renderer Data
  appSpec.rendererType = RendererType::OpenGL;
  
  // Window Specification Data
  appSpec.windowSpecificaion.title = "Kreator";
  appSpec.windowSpecificaion.width = 2500;
  appSpec.windowSpecificaion.height = 1000;
  appSpec.windowSpecificaion.hideTitleBar = true;
#ifdef DEBUG
  appSpec.windowSpecificaion.isFullScreen = false;
#else
  appSpec.windowSpecificaion.isFullScreen = true;
#endif
  
  // Window Controller Data
  appSpec.resizable = true;
  appSpec.startMaximized = true;

  // Ini file
  appSpec.iniFilePath = "../../../Kreator/Kreator.ini";

  // Editor Data --------------------------------------------------------------------------
  // TODO: Extract from arguments
  Kreator::KreatorDirectories kreatorDirectories;
  kreatorDirectories.clientResourcePath = "../../../Kreator/Resources";
  kreatorDirectories.systemUserPath = "/Users/ashish./iKan_storage";
  kreatorDirectories.iKanKreatePath = "/Users/ashish./iKan_storage/Github/Product/IKan-Kreate";

  // Return the Application
  return IKan::Application::CreateApplication<Kreator::KreatorApp>(appSpec, kreatorDirectories);
}
