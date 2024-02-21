//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"
#include "Panel/KreatorConsolePanel.hpp"
#include "Panel/ContentBrowserPanel.hpp"
#include "Panel/ProjectSettingPanel.hpp"
#include "Panel/AssetPanel.hpp"
#include "Editor/AssetViewer.hpp"

namespace Kreator
{
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return

  // Kretor Resource Path
#define KreatorResourcePath(path) std::filesystem::absolute(m_clientResourcePath / path)
  
  // Panel IDs
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define PROJECT_SETTING_PANEL_ID "ProjectSetting"
#define ASSET_MANAGER_PANEL_ID "Assets"

  namespace KreatorUtils
  {
    /// This function replace the project name
    /// - Parameters:
    ///   - str: Project file content
    ///   - projectName: New project name
    static void ReplaceProjectName(std::string& str, const std::string& projectName)
    {
      static const char* projectNameToken = "$PROJECT_NAME$";
      size_t pos = 0;
      while ((pos = str.find(projectNameToken, pos)) != std::string::npos)
      {
        str.replace(pos, strlen(projectNameToken), projectName);
        pos += strlen(projectNameToken);
      }
    }
  } // namespace Utils

  // Viewport -----------------------------------------------------------------------------------------------------
  void Viewport::UpdateMousePos()
  {
    IK_PERFORMANCE("Viewport::UpdateMousePos");
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    
    my = height - my;
    
    mousePosX = (int32_t)mx;
    mousePosY = (int32_t)my;
  }

  // Kreator Layer ------------------------------------------------------------------------------------------------
  KreatorLayer* KreatorLayer::s_instance = nullptr;
  KreatorLayer& KreatorLayer::Get()
  {
    return *s_instance;
  }

  KreatorLayer::KreatorLayer(const std::filesystem::path& clientResourcePath, const std::filesystem::path& systemUserPath, const std::filesystem::path& iKanKreatePath, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath), m_systemUserPath(systemUserPath), m_iKanKreatePath(iKanKreatePath), m_userPreferences(userPreferences), m_viewportRenderer("Viewport"), m_miniViewportRenderer("Mini Viewport")
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Load Textures --------------------
    // Set the Application Icon
    m_applicationIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    // Other Icons
    m_newProjectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folderIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    
    // Shadow Image
    m_shadowTexture = TextureFactory::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));
    
    // Project Icon
    m_projectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Project.png"));
    
    // Window Icons
    m_iconClose = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Attaching '{0} Layer' to application", GetName());
    
    // Decorate the Application --------------------------------------------------------------------------------------
    // Set all the required Fonts
    IKan::UI::ImGuiFont regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    IKan::UI::ImGuiFont boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    IKan::UI::ImGuiFont italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    IKan::UI::ImGuiFont sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    IKan::UI::ImGuiFont hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    IKan::UI::ImGuiFont semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    
    // Note: This API should be called before any other ImGui Decoration API
    Kreator::UI::LoadFonts({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});
    
    // Set the Theme of ImGui as user preference
    Kreator::UI::SetThemeColors(m_userPreferences->theme);
    
    // Save the default project path
    std::filesystem::path defulatPath =  Utils::FileSystem::IKanAbsolute(m_clientResourcePath);
    m_projectFilePathBuffer.MemCpy(defulatPath.string().data(), 0, defulatPath.string().size());
    
    // Open or Create Project ---------------------------------------------------------------------------------------
    if (std::filesystem::exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        IK_ASSERT(false, "Open Project. TODO: Implement Later ...");
      }
    }
    else
    {
      m_showWelcomePopup = true;
    }
    
    // Add Panels -----------------------
    m_panels.AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
    m_panels.AddPanel<ProjectSettingsPanel>(PROJECT_SETTING_PANEL_ID, "Project Setting", true);
    m_panels.AddPanel<AssetPanel>(ASSET_MANAGER_PANEL_ID, "Assets", true);
#if DEBUG == 1
    m_panels.AddPanel<KreatorConsolePanel>(CONSOLE_PANEL_ID, "Editor Log", true);
#endif
    
    // Register Default Asset Editor
    AssetEditorManager::RegisterEditor<ImageViewer>(AssetType::Image);
  }
  
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
    
    // Clear the Asset Editor
    AssetEditorManager::Clear();
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    // Update Asset Viewer
    AssetEditorManager::OnUpdate(ts);

    // Update Data
    m_viewport.UpdateMousePos();
    
    // Render
    // TODO: Move to scene class
    {
      m_viewportRenderer.BeginScene({});
      m_viewportRenderer.EndScene();
    }
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    m_panels.OnEvent(event);
    AssetEditorManager::OnEvent(event);
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    // Pop-up ------------------
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    RETRUN_IF_NO_PROJECT();

    // Docked Windows-----------
    UI_StartMainWindowDocking();
    UI_Viewport();
    
    m_panels.OnImGuiRender();

    // Should be rendered last inside docker
    UI_StatisticsPanel();
    UI_EndMainWindowDocking();
    
    AssetEditorManager::OnImGuiRender();
  }

  void KreatorLayer::UpdateViewportSize()
  {
    m_viewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    m_miniViewportRenderer.SetViewportSize(m_viewport.width, m_viewport.height);
    FixedCamera::SetViewport(m_viewport.width, m_viewport.height);
  }

  void KreatorLayer::CreateProject(const std::filesystem::path& projectDir)
  {
    IK_PROFILE();
    if (!std::filesystem::exists(projectDir))
    {
      IK_LOG_INFO("Kreator Layer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(projectDir).string());
      
      // Close the current Project
      if (Project::GetActive())
      {
        CloseProject();
      }
      
      // Copy the template files
      std::filesystem::path templateProjectDir = m_clientResourcePath / "TemplateProject";
      if (Utils::FileSystem::Copy(templateProjectDir, projectDir))
      {
        // Open Template Project file
        std::ifstream stream(projectDir / "TemplateProject.ikproj");
        std::stringstream ss;
        ss << stream.rdbuf();
        stream.close();
        
        // Rename the Project name in file
        std::string str = ss.str();
        KreatorUtils::ReplaceProjectName(str, m_projectNameBuffer);
        
        // Open Project file again
        std::ofstream ostream(projectDir / "TemplateProject.ikproj");
        ostream << str;
        ostream.close();
        
        // Rename the file name
        std::string newProjectFileName = std::string(m_projectNameBuffer) + ProjectExtension;
        Utils::FileSystem::Rename(projectDir / "TemplateProject.ikproj", projectDir / newProjectFileName);
        
        // Create Required Directories
        std::filesystem::create_directory(projectDir / "Assets/Textures");
        std::filesystem::create_directory(projectDir / "Assets/Fonts");
        std::filesystem::create_directory(projectDir / "Assets/Scenes");
        std::filesystem::create_directory(projectDir / "Assets/Meshes");
        std::filesystem::create_directory(projectDir / "Assets/Materials");
      }
      else
      {
        IK_LOG_CRITICAL("Kreator Layer", "Directory could not be opened. Source directory : {0}. Destination directory : {1}",
                        templateProjectDir.string(), projectDir.string());
      }
    } // If project not exist
    
    std::filesystem::path projectFile = projectDir / (std::string(m_projectNameBuffer) + ProjectExtension);
    OpenProject(projectFile);
  }
  
  void KreatorLayer::CloseProject()
  {
    IK_ASSERT(false);
  }
  
  void KreatorLayer::OpenProject(const std::filesystem::path &filepath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Opening Project {0}", Utils::FileSystem::IKanAbsolute(filepath).string());
    
    // Check if File exists
    if (!std::filesystem::exists(filepath))
    {
      IK_ASSERT(false, "Tried to open a project that doesn't exist. Need to fix ...");
    }

    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }

    // Create new project fill the config with file data
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);
    serializer.Deserialize(filepath);
    Project::SetActive(project);
    
    // Update all panel project
    m_panels.OnProjectChanged(project);

    // Push the current project in recent list
    PushProjectToRecentProjects(filepath);
  }
  
  void KreatorLayer::OpenProject()
  {
    IK_PROFILE();
    RETURN_IF (m_openProjectPath == "")
    
    // stash the filepath away.  Actual opening of project is deferred until it is "safe" to do so.
    m_projectFilePathBuffer.StrCpy(m_openProjectPath.string());
    OpenProject(m_openProjectPath);
  }
  
  void KreatorLayer::PushProjectToRecentProjects(const std::filesystem::path &projectPath)
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Pushing Project {0} in Recent list", Utils::FileSystem::IKanAbsolute(projectPath).string());
    RecentProject projectEntry;
    projectEntry.name = Utils::String::RemoveExtension(projectPath.filename().string());
    projectEntry.filePath = projectPath;
    projectEntry.lastOpened = time(NULL);
    
    for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
    {
      if (it->second.name == projectEntry.name)
      {
        m_userPreferences->recentProjects.erase(it);
        break;
      }
    }
    
    m_userPreferences->recentProjects[projectEntry.lastOpened] = projectEntry;
    UserPreferencesSerializer serializer(m_userPreferences);
    serializer.Serialize(m_userPreferences->filePath);
  }

  const std::filesystem::path& KreatorLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
  const std::filesystem::path& KreatorLayer::GetSystemUserPath() const
  {
    return m_systemUserPath;
  }
  const std::filesystem::path& KreatorLayer::GetIKanKreatorPath() const
  {
    return m_iKanKreatePath;
  }
} // namespace Kreator
