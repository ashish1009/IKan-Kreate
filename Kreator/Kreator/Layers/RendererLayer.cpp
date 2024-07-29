//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#include "RendererLayer.hpp"

#include "Application/Kreator.hpp"

#include "Editor/FolderExplorer.hpp"

#include "Panels/ContentBrowserPanel.hpp"
#include "Panels/KreatorConsolePanel.hpp"
#include "Panels/ProjectSettingPanel.hpp"

namespace Kreator
{
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return

  // Kretor Resource Path
#define KreatorResourcePath(path) std::filesystem::absolute(m_directories.clientResourcePath / path)

  // Panel IDs
#define CONTENT_BROWSER_PANEL_ID "ContentBrowserPanel"
#define CONSOLE_PANEL_ID "EditorConsolePanel"
#define PROJECT_SETTING_PANEL_ID "ProjectSetting"

  namespace KreatorUtils
  {
#define ScopedRounding(value) UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, value);
    
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
  
  RendererLayer* RendererLayer::s_instance = nullptr;
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }
  
  RendererLayer::RendererLayer(const KreatorDirectories& directories, Ref<UserPreferences> userPreferences)
  : Layer("Kreator Renderer"), m_directories(directories), m_viewportRenderer("Primary Viewport"), m_miniViewportRenderer("Mini Viewport"),
  m_secondaryViewportRenderer("Secondary Viewport"), m_userPreferences(userPreferences)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("RendererLayer", "Creating '{0} Layer' instance", GetName());
    
    // Load Textures -------------------------------------------------------------------------------------------------
    m_applicationIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = TextureFactory::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));
    
    m_newProjectIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folderIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    
    m_shadowTexture = TextureFactory::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));

    m_2DIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/2DIcon.png"));
    m_3DIcon = TextureFactory::Create(KreatorResourcePath("Textures/Icons/3DIcon.png"));
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("RendererLayer", "Destroying '{0} Layer' instance", GetName());
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Attaching '{0} Layer' to application", GetName());
    
    // Create the Editor logger -------------------------------------------------------------------------------------
    Logger::Create(LoggerSpecification::Create().
                   Type(LogType::Editor).
                   Level(LogLevel::Trace).
                   Name("PROFILER").
                   OverrideSink(CreateRef<EditorConsoleSink>(1)));
    
    // Decorate the Application ---------------------------------------------------------------------------------------
    // Note: This API should be called before any other ImGui Decoration API
    Kreator::UI::Font::Load({
      // Normal
      {UI::FontType::Regular,    {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14}},
      {UI::FontType::Bold,       {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14}},
      {UI::FontType::Italic,     {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14}},
      
      // Fixed Width
      {UI::FontType::FixedWidth, {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10}},
      
      // Header
      {UI::FontType::HugeHeader, {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40}},
      {UI::FontType::SemiHeader, {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18}}
    });

    // Set the Imgui theme color
    Kreator::UI::Color::SetTheme(m_userPreferences->theme);

    // Open or Create Project ---------------------------------------------------------------------------------------
    auto windowResizeToUnit = [this]()
    {
      if (!Application::Get().GetWindow()->IsFullscreen())
      {
        m_windowOriginalWidth = Application::Get().GetWindow()->GetWidth();
        m_windowOriginalHeight = Application::Get().GetWindow()->GetHeight();
        Window* window = Application::Get().GetWindow();
        window->SetSize(1, 1);
      }
    };
    if (std::filesystem::exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        windowResizeToUnit();
        m_welcomePopup.Set("Welcome Screen", true /* open flag */, 900.0f, 410.0f, true /* center */);
      }
      else
      {
        IK_ASSERT(false, "Open Project. TODO: Implement Later ...");
      }
    }
    else
    {
      windowResizeToUnit();
      m_welcomePopup.Set("Welcome Screen", true /* open flag */, 900.0f, 410.0f, true /* center */);
    }
    
    // Add Panels -----------------------
    m_panels.AddPanel<ContentBrowserPanel>(CONTENT_BROWSER_PANEL_ID, "Content Browser", true);
    m_panels.AddPanel<ProjectSettingsPanel>(PROJECT_SETTING_PANEL_ID, "Project Setting", true);

#ifdef IK_DEBUG
    m_panels.AddPanel<KreatorConsolePanel>(CONSOLE_PANEL_ID, "Editor Log", true);
#endif
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Detaching '{0} Layer' from application", GetName());
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    RETRUN_IF_NO_PROJECT();
    
    m_editorCamera.OnUpdate(ts);
    m_editorCamera.SetActive(true);
    
    m_viewportRenderer.BeginScene();
    m_viewportRenderer.EndScene();
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    RETRUN_IF_NO_PROJECT();
    
    m_editorCamera.OnEvent(event);
    m_panels.OnEvent(event);
  }
  
  void RendererLayer::OnImGuiRender()
  {
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    RETRUN_IF_NO_PROJECT();
    
    // Docked Windows-----------
    UI_StartMainWindowDocking();

    UI_Viewport();
    m_panels.OnImGuiRender();
    
    // To be rendered at last
    UI_StatisticsPanel();

    UI_EndMainWindowDocking();
  }
  
  const std::filesystem::path& RendererLayer::GetClientResorucePath() const
  {
    return m_directories.clientResourcePath;
  }
  const std::filesystem::path& RendererLayer::GetSystemUserPath() const
  {
    return m_directories.systemUserPath;
  }
  const std::filesystem::path& RendererLayer::GetIKanKreatorPath() const
  {
    return m_directories.kreatorPath;
  }
  
  void RendererLayer::UpdateViewportSize()
  {
    IK_PROFILE()
    
    FixedCamera::SetViewport(m_primaryViewport.width, m_primaryViewport.height);
    
    m_editorCamera.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    
    m_viewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_miniViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_secondaryViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
  }
  
  void RendererLayer::CreateProject(const std::filesystem::path &newProjectFilePath)
  {
    IK_PROFILE();
    
    // If file do not exist, create the directory
    if (!std::filesystem::exists(newProjectFilePath))
    {
      IK_LOG_INFO("RendererLayer", "Creating Project at {0} ", Utils::FileSystem::IKanAbsolute(newProjectFilePath).string());
      
      // Close current project
      if (Project::GetActive())
      {
        CloseProject();
      }
      
      // Create project directory
      // Copy the template files
      std::filesystem::path templateProjectDir = m_directories.clientResourcePath / "TemplateProject";
      if (Utils::FileSystem::Copy(templateProjectDir, newProjectFilePath))
      {
        // Open Template Project file
        std::ifstream stream(newProjectFilePath / "TemplateProject.ikproj");
        std::stringstream ss;
        ss << stream.rdbuf();
        stream.close();
        
        // Rename the Project name in file
        std::string str = ss.str();
        KreatorUtils::ReplaceProjectName(str, newProjectFilePath.filename());
        
        // Open Project file again
        std::ofstream ostream(newProjectFilePath / "TemplateProject.ikproj");
        ostream << str;
        ostream.close();
        
        // Rename the file name
        std::string newProjectFileName = std::string(newProjectFilePath.filename()) + ProjectExtension;
        Utils::FileSystem::Rename(newProjectFilePath / "TemplateProject.ikproj", newProjectFilePath / newProjectFileName);
        
        // Create Required Directories
        std::filesystem::create_directory(newProjectFilePath / "Assets/Textures");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Fonts");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Scenes");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Meshes");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Materials");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Physics");
        std::filesystem::create_directory(newProjectFilePath / "Assets/Prefabs");
        
        // Add more based on Project config
      }
      else
      {
        IK_LOG_CRITICAL("RendererLayer", "Directory could not be opened. Source directory : {0}. Destination directory : {1}",
                        templateProjectDir.string(), newProjectFilePath.string());
      }
    }
    
    // Open project if directory exists
    std::filesystem::path projectFile = newProjectFilePath / (std::string(newProjectFilePath.filename()) + ProjectExtension);
    OpenProject(projectFile);
  }
  
  void RendererLayer::OpenProject(const std::filesystem::path &projectFilePath)
  {
    IK_PROFILE();
    IK_LOG_INFO("RendererLayer", "Opening Project {0}", Utils::FileSystem::IKanAbsolute(projectFilePath).string());
    
    // Check if File exists
    if (!std::filesystem::exists(projectFilePath))
    {
      IK_ASSERT(false, "Tried to open a project that doesn't exist. Need to fix ...");
    }
    
    // Check if File is valid kreator project
    if (projectFilePath.extension() != ProjectExtension)
    {
      IK_ASSERT(false, "Invalid project file. Need to fix ...");
    }
    
    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }
    
    // Create new project fill the config with file data
    Ref<Project> project = CreateRef<Project>();
    ProjectSerializer serializer(project);
    serializer.Deserialize(projectFilePath);
    Project::SetActive(project);
    
    // Update all panel project
    m_panels.OnProjectChanged(project);

    // Push the current project in recent list
    PushProjectToRecentProjects(projectFilePath);
    
    // Once project is created resize the window to original size
    if (!Application::Get().GetWindow()->IsFullscreen())
    {
      Window* window = Application::Get().GetWindow();
      window->SetSize(m_windowOriginalWidth, m_windowOriginalHeight);
    }
  }
  
  void RendererLayer::CloseProject()
  {
    IK_PROFILE();
    IK_ASSERT(false, "Implement Later");
  }
  
  void RendererLayer::PushProjectToRecentProjects(const std::filesystem::path &projectPath)
  {
    IK_PROFILE();
    IK_LOG_INFO("RendererLayer", "Pushing Project {0} in Recent list", Utils::FileSystem::IKanAbsolute(projectPath).string());
    
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
  
  // UI APIs ---------------------------------------------------------------------------------------------------------
  void RendererLayer::UI_WelcomePopup()
  {
    m_welcomePopup.Show(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar, [this]()
                        {
      // Create Table in popup
      UI::ScopedTable welcomeTable({{"##About", 600.0f}, {"##RecentProjects", -1 /* streched to width */}});

      // Background color as popup BG
      UI::ScopedColor childBG(ImGuiCol_ChildBg, UI::Color::PopupBackground);
      
      // About/New_Project Column -----------------------------------------------------
      welcomeTable.ShowColumn(0, [this]()
                              {
        // Kreator Logo and title
        {
          // Icon
          static constexpr glm::vec2 logoSize {200, 200};
          UI::Image(m_welcomeIcon, logoSize, UI::AlignX::Center);
          
          // Welcome Header
          UI::Text(UI::FontType::HugeHeader, "Welcome to Kreator", UI::AlignX::Center, {0.0f, logoSize.y});

          // Version
          static std::string versionText = "Version v" + Application::GetVersion() + "." + KreatorApp::GetVersion();
          UI::Text(UI::FontType::Italic, versionText.c_str(), UI::AlignX::Center);

          ImGui::Separator();
        }
        
        // Buttons
        {
          static const glm::vec2 offset = {50.0f, 0.0f};
          static const glm::vec2 buttonSize = {40.0f, 40.0f};
          
          if (UI::DrawImageTextButton("Create New Kreator Project", m_newProjectIcon, buttonSize, offset))
          {
            ImGui::CloseCurrentPopup();
            m_createNewProjectPopup.Set("Create New Project", true /* open Flag */, 650, 0, true /* center */);
          }
          
          if (UI::DrawImageTextButton("Open an exisiting Kreator Project", m_folderIcon, buttonSize, offset))
          {
            ImGui::CloseCurrentPopup();
            FolderExplorer::ShowOpenPopup(ProjectExtension, m_directories.kreatorPath / "Kreator/Projects" , &m_welcomePopup);
            m_folderExplorerAction = FolderExplorerAction::OpenProject;
          }
        }
        
        // Show again check box
        {
          ScopedRounding(5);
          UI::Separator();
          UI::ShiftCursorY(10);
          if (ImGui::Checkbox("##showAgain", &m_userPreferences->showWelcomeScreen))
          {
            UserPreferencesSerializer serializer(m_userPreferences);
            serializer.Serialize(m_userPreferences->filePath);
          }
          
          UI::SameLine();
          ImGui::TextUnformatted("Show this window again when Kreator Launches");
        }
        
        // Shadow
        UI::DrawShadowInner(m_shadowTexture);

      });
      
      welcomeTable.ShowColumn(1, [this]() {
        // Title of Popup --------------------------------------------------
        UI::Text(UI::FontType::SemiHeader, "Recent Projects", UI::AlignX::Center);
        ImGui::Separator();
        
        // Recent Project --------------------------------------------------
        {
          UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 10.0f));
          UI::ScopedFont version(UI::Font::Get(UI::FontType::Bold));
          
          for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
          {
            if (!std::filesystem::exists(it->second.filePath))
            {
              m_userPreferences->recentProjects.erase(it);
              break;
            }
            
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
            bool open = UI::TreeNode("##Recent_Projects", it->second.name, flags, m_newProjectIcon);
            if (ImGui::IsItemClicked())
            {
              OpenProject(it->second.filePath);
              ImGui::CloseCurrentPopup();
              break;
            }
            
            if(open)
            {
              ImGui::TreePop();
            }
          }
        }
      });
    });
  }
  
  void RendererLayer::UI_NewProjectPopup()
  {
    m_createNewProjectPopup.Show(ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar , [this]()
                                 {
      // Title of Popup
      UI::Text(UI::FontType::Bold, "Choose a template for your project", UI::AlignX::Left, {0.0f, 0.0f}, UI::Color::TextBrighter);
      ImGui::Separator();
      
      // Icons for 2D 3D projects
      static ImU32 tintColor3DIcon = UI::Color::TextMuted;
      static ImU32 tintColor2DIcon = UI::Color::TextMuted;

      static ImU32 createButtonTextColor = UI::Color::TextBrighter;
      static ImU32 createButtonColor = UI::Color::TextMuted;
      static std::string createButtonTitle = " Create ";

      switch (m_selectedProject)
      {
        case SelectedProject::_2D:
          // Icon Color
          tintColor2DIcon = UI::Color::NiceBlueHighlight;
          tintColor3DIcon = UI::Color::TextMuted;
          
          // Create button
          createButtonTextColor = UI::Color::Background;
          createButtonColor = UI::Color::NiceBlueHighlight;
          createButtonTitle = " Create 2D ";
          break;
          
        case SelectedProject::_3D:
          // Icon Color
          tintColor2DIcon = UI::Color::TextMuted;
          tintColor3DIcon = UI::Color::NiceBlueHighlight;
          
          // Create button
          createButtonTextColor = UI::Color::Background;
          createButtonColor = UI::Color::NiceBlueHighlight;
          createButtonTitle = " Create 3D ";
          break;
          
        case SelectedProject::None:
        default:
          // Icon Color
          tintColor2DIcon = UI::Color::TextMuted;
          tintColor3DIcon = UI::Color::TextMuted;
          
          // Create button
          createButtonTextColor = UI::Color::TextBrighter;
          createButtonColor = UI::Color::TextMuted;
          createButtonTitle = " Create ";
          break;
      }
      
      // Draw the icons of 2D and 3D
      static const glm::vec2 size = {150.0f, 150.0f};
      if (UI::DrawButtonImage("2D", m_2DIcon, size, {100.0f, 20.0f}, true, tintColor2DIcon, UI::Color::TextBrighter, tintColor2DIcon))
      {
        m_selectedProject = SelectedProject::_2D;
      }
      ImGui::SameLine();
      if (UI::DrawButtonImage("3D", m_3DIcon, size, {100.0f, 0.0f}, true, tintColor3DIcon, UI::Color::TextBrighter, tintColor3DIcon))
      {
        m_selectedProject = SelectedProject::_3D;
      }
      
      UI::ShiftCursorY(20);
      ImGui::Separator();
      
      // Cancel Button
      if (UI::DrawButton(" Cancel ", UI::FontType::Bold, UI::Color::TextBrighter, UI::Color::TextMuted, 2) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
      {
        if (!Project::GetActive())
        {
          m_welcomePopup.SetFlag(true);
        }
        m_selectedProject = SelectedProject::None;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::CalcTextSize(createButtonTitle.c_str()).x - 20);
      
      // Create Button
      if (UI::DrawButton(createButtonTitle, UI::FontType::Bold, createButtonTextColor, createButtonColor, 2) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
      {
        if (m_selectedProject != SelectedProject::None)
        {
          FolderExplorer::ShowCreatePopup(m_directories.kreatorPath / "Kreator", &m_createNewProjectPopup);
          m_folderExplorerAction = FolderExplorerAction::NewProject;
          m_selectedProject = SelectedProject::None;
          ImGui::CloseCurrentPopup();
        }
      }
    });
  }
  
  void RendererLayer::UI_FolderExplorer()
  {
    std::filesystem::path explorerOutput = FolderExplorer::Explore();
    if (explorerOutput == "")
    {
      return;
    }
    
    IK_PERFORMANCE("RendererLayer::UI_FolderExplorer");
    switch (m_folderExplorerAction)
    {
      case FolderExplorerAction::NewProject:
      {
        CreateProject(explorerOutput);
        break;
      }
      case FolderExplorerAction::OpenProject:
      {
        if (explorerOutput.extension() == ProjectExtension)
        {
          OpenProject(explorerOutput);
        }
        else
        {
          IK_ASSERT(false, "Should never reach here...");
        }
        break;
      }
      case FolderExplorerAction::None:
      default:
        IK_ASSERT(false);
        break;
    }
  }

  void RendererLayer::UI_StartMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_StartMainWindowDocking");
    static bool optFullscreenPersistant = true;
    static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
    bool optFullscreen = optFullscreenPersistant;
    
    // ImGui Input Output Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
      windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);
    
    if (optFullscreen)
    {
      ImGui::PopStyleVar(2);
    }
    
    // Dockspace
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
  }
  
  void RendererLayer::UI_EndMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_EndMainWindowDocking");
    ImGui::End();
  }
  
  void RendererLayer::UI_Viewport()
  {
    IK_PERFORMANCE("RendererLayer::UI_Viewport")
    
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    
    m_primaryViewport.isHovered = ImGui::IsWindowHovered();
    m_primaryViewport.isFocused = ImGui::IsWindowFocused();
    
    const ImVec2& size = ImGui::GetContentRegionAvail();
    
    // Updating the View port size and kreator data w.r.t. primary viewport
    bool zeroSizeViewport = 0 == size.x or 0 == size.y;
    bool sameSizeViewport = size.x == m_primaryViewport.width and size.x == m_primaryViewport.height;
    if(!zeroSizeViewport and !sameSizeViewport)
    {
      m_primaryViewport.width = static_cast<uint32_t>(size.x);
      m_primaryViewport.height = static_cast<uint32_t>(size.y);
      
      // Set Viewport of Dummy Data
      UpdateViewportSize();
    }
    
    // Render viewport image
    UI::Image(m_viewportRenderer.GetFinalImage(), size);
    
    ImGui::End();
  }
  
  void RendererLayer::UI_StatisticsPanel()
  {
    IK_PERFORMANCE("KreatorLayer::UI_StatisticsPanel");
    if (!m_showStatisticsPanel)
    {
      return;
    }
    
    if (ImGui::Begin("Statistics", &m_showStatisticsPanel))
    {
      UI::ScopedColor header(ImGuiCol_Header, UI::Color::PopupBackground);
      UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 5);
      
      ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
      if (ImGui::BeginTabBar("StatisticsTabs", tabBarFlags))
      {
        if (ImGui::BeginTabItem("Renderer"))
        {
          UI::ScopedDisable disable;
          auto& caps = RendererCapabilities::Get();
          UI::BeginPropertyGrid(2, 1, 1);
          UI::Property("Vendor", caps.vendor.data());
          UI::Property("Renderer", caps.renderer.data());
          UI::Property("Version", caps.version.data());
          UI::EndPropertyGrid();
          ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Performance"))
        {
          {
            ImGui::PushID("FPS");
            UI::ScopedDisable disable;
            UI::BeginPropertyGrid(2, 1, 1);
            UI::Property("FPS", ImGui::GetIO().Framerate);
            UI::EndPropertyGrid();
            ImGui::PopID();
          }
          
          // Search Widget
          static char searchBuffer[256];
          ImGui::SetNextItemWidth(-1);
          ImGui::InputTextWithHint("##regsearch", "Search ...", searchBuffer, 256);
          
          ImGui::Separator();
          
          {
            ImGui::PushID("Perf Results");
            UI::BeginPropertyGrid(2, 1, 1);
            
            static float timeLimit = 0.0f;
            UI::Property("Min Time Limit", timeLimit, 0.001f, 0.0f, 1000.0f);
            
            UI::ScopedDisable disable;
            const auto& perFrameData = PerformanceProfiler::Get().GetPerFrameData();
            for (auto&& [name, time] : perFrameData)
            {
              if ((!UI::IsMatchingSearch(name, searchBuffer) and !UI::IsMatchingSearch(std::to_string(time), searchBuffer)) or time < timeLimit)
              {
                continue;
              }
              UI::Property(name, std::to_string(time));
              UI::SetTooltip(name);
            }
            UI::EndPropertyGrid();
            ImGui::PopID();
          }
          ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Renderer Stats"))
        {
          const auto& stats = RendererStatistics::Get();
          
          if (ImGui::BeginTabBar("Renderer Stats Tabs", tabBarFlags))
          {
            if (ImGui::BeginTabItem("General Stats"))
            {
              UI::ScopedDisable disable;
              UI::BeginPropertyGrid(2, 1, 1);
              UI::Property("Draw Calls", std::to_string(stats.drawCalls));
              UI::Property("Vertex Count", std::to_string(stats.vertexCount));
              UI::Property("Vertex Buffer Size", std::to_string((uint32_t)(stats.vertexBufferSize/1000)));
              UI::Property("Index Count", std::to_string(stats.indexCount));
              UI::Property("Index Buffer Size", std::to_string((uint32_t)(stats.indexBufferSize / 1000)));
              UI::Property("Texture Buffer Size", std::to_string((uint32_t)(stats.textureBufferSize / 1000)));
              UI::EndPropertyGrid();
              ImGui::EndTabItem();
            }
            
            if (ImGui::BeginTabItem("2D Stats"))
            {
              UI::ScopedDisable disable;
              UI::BeginPropertyGrid(2, 1, 1);
              UI::Property("Quads in this batch   ", std::to_string(stats._2d.quads));
              UI::Property("Max Quad Per Batch    ", std::to_string(stats._2d.maxQuads));
              UI::Property("Circles in this batch ", std::to_string(stats._2d.circles));
              UI::Property("Max Circles Per Batch ", std::to_string(stats._2d.maxCircles));
              UI::Property("Lines in this batch   ", std::to_string(stats._2d.lines));
              UI::Property("Max Lines Per Batch   ", std::to_string(stats._2d.maxLines));
              UI::Property("Chars in this batch   ", std::to_string(stats._2d.chars));
              UI::Property("Max Char Per Batch    ", std::to_string(16));
              UI::EndPropertyGrid();
              ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
          }
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
    }
    ImGui::End();
  }
} // namespace Kreator
