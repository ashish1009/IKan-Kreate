//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"

extern std::string IKanVersion;

namespace Kreator
{
#define RETRUN_IF_NO_PROJECT() \
if (!Project::GetActive()) return
  
  // Kretor Resource Path
#define KreatorResourcePath(path) Utils::FileSystem::Absolute(s_clientResourcePath / path)

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

  // Kreator UI utils
  namespace UI_Utils
  {
    // Hovered Item Color
    static const ImU32 s_hoveredColor = Kreator_UI::Color::HoveredItem;
    
    // Function to push Dark color on active
    static const auto pushDarkTextIfActive = [](const char* menuName)
    {
      if (ImGui::IsPopupOpen(menuName))
      {
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Theme::Color::BackgroundDark);
        return true;
      }
      return false;
    };
    
    /// This function Create template for Menu Item in Menu Bar
    /// - Parameters:
    ///   - title: Title of Menu
    ///   - func: function of Menu
    ///   - pop_func: Pop item highlight
    template<typename PopFunction, typename UIFunction>
    void AddMenu(const char* title, PopFunction popItemHighlight, UIFunction menuFunc)
    {
      // Change Text colored to dark if item is opened
      bool pushItemColor = pushDarkTextIfActive(title);
      
      if (ImGui::BeginMenu(title))
      {
        popItemHighlight();
        {
          UI::ScopedColor hovered(ImGuiCol_HeaderHovered, s_hoveredColor);
          menuFunc();
        }
        ImGui::EndMenu();
      }
      if (pushItemColor)
      {
        ImGui::PopStyleColor();
      }
    }
    
  } // namespace UI_Utils
  
  void Viewport::UpdateMousePos()
  {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    
    my = height - my;
    
    mousePosX = (int32_t)mx;
    mousePosY = (int32_t)my;
  }
  
  std::pair<float, float> Viewport::GetMouseSpace()
  {
    auto [mx, my] = ImGui::GetMousePos();
    mx -= bounds[0].x;
    my -= bounds[0].y;
    auto viewportWidth = bounds[1].x - bounds[0].x;
    auto viewportHeight = bounds[1].y - bounds[0].y;
    
    return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  
  std::pair<float, float> Viewport::GetMousePos()
  {
    return { mousePosX, mousePosY };
  }
  
  std::filesystem::path RendererLayer::s_clientResourcePath;
  
  std::filesystem::path RendererLayer::GetClientResorucePath()
  {
    return s_clientResourcePath;
  }

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
  , m_userPreferences(userPreference)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    s_clientResourcePath = clientResourcePath;
    m_allProjectsPath = s_clientResourcePath / "Projects";
    
    m_projectNameBuffer = iknew char[MAX_PROJECT_NAME_LENGTH];
    m_projectFilePathBuffer = iknew char[MAX_PROJECT_FILEPATH_LENGTH];
    
    // Save the default project path
    auto fullAllProjectPath =  Utils::FileSystem::IKanAbsolute(m_allProjectsPath);
    memccpy(m_projectFilePathBuffer, fullAllProjectPath.data(), 0, fullAllProjectPath.size());
    
    m_templateProjectDir = s_clientResourcePath / "TemplateProject";
    
    // Set the Application Icon
    m_applicationIcon = Image::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    m_welcomeIcon = Image::Create(KreatorResourcePath("Textures/Logo/WelcomeIKan.png"));

    // Window Icons
    m_iconClose = Image::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = Image::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = Image::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = Image::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
    
    // Shadow Icon
    m_shadowTexture = Image::Create(KreatorResourcePath("Textures/Icons/ShadowLineTop.png"));
    
    // Other Icons
    m_newProject = Image::Create(KreatorResourcePath("Textures/Icons/NewProject.png"));
    m_folder = Image::Create(KreatorResourcePath("Textures/Icons/Folder.png"));
    m_projectIcon = Image::Create(KreatorResourcePath("Textures/Icons/Project.png"));
  }
  
  RendererLayer::~RendererLayer()
  {
    ikdelete m_projectNameBuffer;
    ikdelete m_projectFilePathBuffer;
    
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
    
    // Decorate the Theme
    UI::Font regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    UI::Font boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    UI::Font italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    UI::Font sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    UI::Font Hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    UI::Font semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    UI::Theme::ChangeFont({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, Hugeheader, semiheader});

    Kreator_UI::SetDarkTheme();

    // Open or Create Project
    if (Utils::FileSystem::Exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        OpenProject(m_userPreferences->startupProject);
      }
    }
    else
    {
      m_showWelcomePopup = true;
    }
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    RETRUN_IF_NO_PROJECT();
    
    m_viewport.UpdateMousePos();

    m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
    m_editorCamera.OnUpdate(ts);

    Renderer2D::BeginRenderPass();
    Renderer::Clear({0.12f, 0.12f, 0.18f, 1.0f});
      
    m_editorScene->OnUpdateEditor(ts);
    m_editorScene->OnRenderEditor(m_editorCamera);

    Renderer2D::EndRenderPass();
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    RETRUN_IF_NO_PROJECT();

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnMouseButtonPressed));

    if (m_viewport.panelMouseHover)
    {
      m_editorCamera.OnEvent(event);
    }
  }
  
  bool RendererLayer::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    if (Input::IsKeyPressed(Key::LeftSuper) and !Input::IsMouseButtonPressed(MouseButton::Right))
    {
      switch (e.GetKeyCode())
      {
        case Key::N:
          NewScene();
          break;
        default:
          break;
      }
    }
    return false;
  }

  bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
  {
    return false;
  }

  void RendererLayer::UpdateViewportSize()
  {
    Renderer2D::SetViewport(m_viewport.width, m_viewport.height);
    m_editorCamera.SetViewportSize(m_viewport.width, m_viewport.height);
  }

  void RendererLayer::OnImguiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImguiRender");
    
    // Should be above all scene GUI
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    RETRUN_IF_NO_PROJECT();
    
    UI_StartMainWindowDocking();
    UI_Viewport();
    UI_EndMainWindowDocking();
  }
  
  void RendererLayer::UpdateWindowTitle(const std::string& sceneName)
  {
    const auto& caps = Renderer::Capabilities::Get();
    const std::string title = fmt::format("{0} ({1}) - Kreator - {2} ({3}) Renderer: {4}",
                                          sceneName, Project::GetActive()->GetConfig().name,
                                          caps.vendor, caps.version, caps.renderer);
    Application::Get().GetWindow().SetTitle(title);
  }
  
  void RendererLayer::CreateProject(const std::filesystem::path &projectDir)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Project {0} at ", projectDir.string().c_str());
    
    // Close the current Project
    if (Project::GetActive())
    {
      CloseProject();
    }
    
    // Copy the template files
    Utils::FileSystem::Copy(m_templateProjectDir, projectDir);
    
    // Open Template Project file
    std::ifstream stream(projectDir / "TemplateProject.ikproj");
    std::stringstream ss;
    ss << stream.rdbuf();
    stream.close();

    // Rename the Project name in file
    std::string str = ss.str();
    KreatorUtils::ReplaceProjectName(str, m_projectNameBuffer);
    
    // Open Projecy file again
    std::ofstream ostream(projectDir / "TemplateProject.ikproj");
    ostream << str;
    ostream.close();

    // Rename the file name
    std::string newProjectFileName = std::string(m_projectNameBuffer) + ProjectExtension;
    Utils::FileSystem::Rename(projectDir / "TemplateProject.ikproj", projectDir / newProjectFileName);
    
    // Create Required Direcotries
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Textures");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Fonts");
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Scenes");
    
    auto projectFile = projectDir.string() + "/" + std::string(m_projectNameBuffer) + ProjectExtension;
    OpenProject(projectFile);
  }
  
  void RendererLayer::OpenProject(const std::string &filepath)
  {
    IK_LOG_TRACE("Kreator Layer", "Opening Project {0}", filepath.c_str());

    // Check if File exists
    if (!Utils::FileSystem::Exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer", "Tried to open a project that doesn't exist. Project path: {0}", filepath);
      
      // TODO: Remove Assert after testing. may be this will never hit just precaution check
      IK_ASSERT(false);
      return;
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
    
    // Reset cameras
    m_editorCamera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
    
    if (!project->GetConfig().startScene.empty())
    {
      OpenScene((Project::GetAssetDirectory() / project->GetConfig().startScene).string());
    }
    else
    {
      NewScene();
    }
    
    PushProjectToRecentProjects(filepath);
  }
  
  void RendererLayer::PushProjectToRecentProjects(std::filesystem::path projectFile)
  {
    RecentProject projectEntry;
    projectEntry.name = Utils::String::RemoveExtension(projectFile.filename().string());
    projectEntry.filePath = projectFile;
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
  
  void RendererLayer::OpenProject()
  {
    if (m_openProjectPath == "")
    {
      return;
    }
    
    // stash the filepath away.  Actual opening of project is deferred until it is "safe" to do so.
    strcpy(m_projectFilePathBuffer, m_openProjectPath.string().c_str());
    
    PushProjectToRecentProjects(m_openProjectPath);
    OpenProject(m_openProjectPath);
  }
  
  void RendererLayer::CloseProject(bool unloadProject)
  {
    SaveProject();
  }
  
  void RendererLayer::SaveProject()
  {
    if (!Project::GetActive())
    {
      // FIXME: (Kreator) Fix Later
      IK_ASSERT(false);
    }
    
    Ref<Project> project = Project::GetActive();
    ProjectSerializer serializer(project);
    serializer.Serialize(project->GetConfig().projectDirectory + "/" + project->GetConfig().projectFileName);
  }
  
  void RendererLayer::NewScene(const std::string& name)
  {
    m_editorScene =  Scene::Create(name);
    UpdateWindowTitle(name);
    m_sceneFilePath = std::string();
    
    m_editorCamera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
    m_currentScene = m_editorScene;
  }
  void RendererLayer::OpenScene(const std::string& filepath)
  {
    if (!Utils::FileSystem::Exists(filepath))
    {
      IK_LOG_ERROR("Kreator Layer" ,"Tried loading a non-existing scene: {0}", filepath);
      IK_ASSERT(false);
    }
    
    Ref<Scene> newScene = Scene::Create("New Scene");
    SceneSerializer serializer(newScene);
    serializer.Deserialize(filepath);
    
    m_editorScene = newScene;
    m_currentScene = m_editorScene;
    m_sceneFilePath = filepath;
    
    std::filesystem::path path = filepath;
    UpdateWindowTitle(path.filename().string());
  }

  // UI APIS ---------------------------------------------------------------------------------------------------------
  void RendererLayer::UI_StartMainWindowDocking()
  {
    static bool optFullscreenPersistant = true;
    static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
    bool optFullscreen = optFullscreenPersistant;
    
    // ImGui Input Output Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = /* ImGuiWindowFlags_MenuBar |*/ ImGuiWindowFlags_NoDocking;
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
    
#if 0
    // When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle
    // the pass-thru hole, so we ask Begin() to not render a background.
    if (optFlags & ImGuiDockNodeFlags_PassthruCentralNode)
    {
      windowFlags |= ImGuiWindowFlags_NoBackground;
    }
#endif

    const Window& window = Application::Get().GetWindow();
    bool isMaximized = window.IsMaximized();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, isMaximized ? ImVec2(6.0f, 6.0f) : ImVec2(3.0f, 3.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);
    
    if (optFullscreen)
    {
      ImGui::PopStyleVar(2);
    }
    
    // Render the title if original title bar is hidden
    if (Application::Get().GetSpecification().windowSpecification.hideTitleBar)
    {
      float titlebarHeight = UI_DrawTitlebar();
      UI::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);
    }
    
    // Dockspace
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 300.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
  }
  
  void RendererLayer::UI_EndMainWindowDocking()
  {
    ImGui::End();
  }
  
  void RendererLayer::UI_Viewport()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    
    m_viewport.panelMouseHover = ImGui::IsWindowHovered();
    m_viewport.panelFocused = ImGui::IsWindowFocused();
    
    auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
    auto viewportSize = ImGui::GetContentRegionAvail();
    
    // Updating the View port size
    m_viewport.width = viewportSize.x;
    m_viewport.height = viewportSize.y;
    
    // Set Viewport of Kreator Data
    UpdateViewportSize();
    
    // Render viewport image
    ImGui::Image(INT2VOIDP(Renderer2D::GetFinalImage()->GetRendererID()), viewportSize, {0, 1}, {1, 0});

    auto windowSize = ImGui::GetWindowSize();
    ImVec2 minBound = ImGui::GetWindowPos();
    minBound.x += viewportOffset.x;
    minBound.y += viewportOffset.y;
    
    ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
    m_viewport.bounds[0] = { minBound.x, minBound.y };
    m_viewport.bounds[1] = { maxBound.x, maxBound.y };
    
    m_allowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
  
  float RendererLayer::UI_DrawTitlebar()
  {
    float titlebarHeight = 40.0f;

    const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + titlebarHeight
    };
    
    UI::SetCursorPos(ImVec2(windowPadding.x, windowPadding.y));
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Theme::Color::Titlebar);

    // Drag and Control the window with user title bar ---------------------------------
    UI::SetCursorPos(windowPadding);
    UI_TitlebarDragArea(titlebarHeight);

    // Draw Kreator Logo ---------------------------------------------------------------
    const int32_t logoWidth = titlebarHeight - 10;
    const int32_t logoHeight = titlebarHeight - 10;
    const ImVec2 logoOffset(windowPadding.x, windowPadding.y);
    const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x + logoOffset.x,
      ImGui::GetItemRectMin().y + logoOffset.y
    };
    const ImVec2 logoRectMax =
    {
      logoRectStart.x + logoWidth,
      logoRectStart.y + logoHeight
    };
    drawList->AddImage(UI::GetTextureID(m_applicationIcon), logoRectStart, logoRectMax);

    // Draw the Menu Tab in Title bar --------------------------------------------------
    ImGui::SetItemAllowOverlap();
      
    const float logoOffsetX = 6.0f * 2.0f + 41.0f + windowPadding.x;
    UI::SetCursorPos(ImVec2(logoOffsetX, 4.0f));
    
    UI_MenuBar();

    // Round Bar -------------------------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() / 4);
    UI::SetCursorPosY(titlebarHeight / 3);

    const ImVec2 roundBarMin = ImGui::GetCursorScreenPos();
    const ImVec2 roundBarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() / 2,
      ImGui::GetCursorScreenPos().y + 23
    };
    drawList->AddRectFilled(roundBarMin, roundBarMax, Kreator_UI::Color::TitleBarDark, 10);
    float roundBarRight = (3 * ImGui::GetWindowWidth()) / 4;
    UI::ShiftCursorY(5);

    const float underlineThickness = 1.0f;
    // Project name --------------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, Kreator_UI::Color::TextDarker);
      UI::ShiftCursorX(10);
      const std::string title = Project::GetActive()->GetConfig().name;
      {
        UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
        ImGui::Text(title.c_str());
      }
      UI::SetTooltip("Current project (" + Project::GetActive()->GetConfig().projectFileName + ")");

      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);
      // Make the Max + thickness of rectange as min of verticle line
      itemRect.Min.x = itemRect.Max.x + underlineThickness;
      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Theme::Color::Muted, 2.0f);
    }
    
    // Current Scene name ---------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, UI::Theme::Color::Text);
      UI::SameLine();

      const std::string sceneName = m_currentScene->GetName();
      UI::SetCursorPosX(roundBarRight - ImGui::CalcTextSize(sceneName.c_str()).x - 20);
      {
        UI::ScopedFont boldFont(Kreator_UI::GetBoldFont());
        ImGui::Text(sceneName.c_str());
      }
      UI::SetTooltip("Current scene (" + m_sceneFilePath + ")");
      
      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);

      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Theme::Color::Muted, 2.0f);
    }
    
    // Render the Window Buttons -------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() - 78);
    UI::SetCursorPosY(20.0f);
    UI_WindowButtons();

    return titlebarHeight;
  }
  
  void RendererLayer::UI_TitlebarDragArea(float titlebarHeight)
  {
    static float moveOffsetX;
    static float moveOffsetY;
    const float titleBarWidth = ImGui::GetContentRegionAvail().x;
    
    auto* rootWindow = ImGui::GetCurrentWindow()->RootWindow;
    const float windowWidth = (int32_t)rootWindow->RootWindow->Size.x;
    
    if (ImGui::InvisibleButton("##titleBarDragZone", ImVec2(titleBarWidth, titlebarHeight * 2),
                               ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_AllowItemOverlap))
    {
      ImVec2 point = ImGui::GetMousePos();
      ImRect rect = rootWindow->Rect();
      
      // Calculate the difference between the cursor pos and window pos
      moveOffsetX = point.x - rect.Min.x;
      moveOffsetY = point.y - rect.Min.y;
    }
    
    const Window& window = Application::Get().GetWindow();
    bool maximized = window.IsMaximized();
    
    // Maximize or restore on doublt click
    if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) and ImGui::IsItemHovered())
    {
      (maximized) ? window.Restore() : window.Maximize();
    }
    else if (ImGui::IsItemActive())
    {
      if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
      {
        if (maximized)
        {
          window.Restore();
          
          // Get the original size of window
          [[maybe_unused]] int32_t newWidth = window.GetWidth();
          [[maybe_unused]] int32_t newHeight = window.GetHeight();
          
          // Offset position proportionally to mouse position on titlebar
          // This ensures we dragging window relatively to cursor position on titlebar
          // correctly when window size changes
          if (windowWidth - (float)newWidth > 0.0f)
          {
            moveOffsetX *= (float)newWidth / windowWidth;
          }
        }
        
        // Update the new position of window
        ImVec2 point = ImGui::GetMousePos();
        window.SetPosition({point.x - moveOffsetX, point.y - moveOffsetY});
      }
    }
  }
  
  void RendererLayer::UI_MenuBar()
  {
    // Menu Bar Rectactangle Size
    const ImRect menuBarRect =
    {
      ImGui::GetCursorPos(), // Min Rect Coord
      {ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()} // Max Rect Coord
    };
    
    ImGui::BeginGroup();
    if (UI::BeginMenuBar(menuBarRect))
    {
      bool menuOpen = ImGui::IsPopupOpen("##menubar", ImGuiPopupFlags_AnyPopupId);
      
      // Push the Colors if Menu is active
      if (menuOpen)
      {
        const ImU32 colActive = UI::ColorWithSaturation(Kreator_UI::Color::Accent, 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Header, colActive);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colActive);
      }
      
      // Function to Pop the highlight Color
      auto popItemHighlight = [&menuOpen]
      {
        if (menuOpen)
        {
          ImGui::PopStyleColor(2);
          menuOpen = false;
        }
      };
      
      // Menu Items
      UI_Utils::AddMenu("File", popItemHighlight, [this]() {
        if (ImGui::MenuItem("Create Project..."))
        {
          m_showCreateNewProjectPopup = true;
        }
        if (ImGui::MenuItem("Open Project...", "Cmd + O"))
        {
          FolderExplorer::OpenPopup(m_allProjectsPath);
          m_folderExplorerAction = FolderExplorerAction::OpenProject;
        }
        if (ImGui::BeginMenu("Open Recent"))
        {
          size_t i = 0;
          for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
          {
            if (i > 10)
            {
              break;
            }
            
            if (Utils::FileSystem::Exists(it->second.filePath))
            {
              if (ImGui::MenuItem(it->second.name.c_str()))
              {
                // stash filepath away and defer actual opening of project until it is "safe" to do so
                strcpy(m_projectFilePathBuffer, it->second.filePath.data());
                
                RecentProject projectEntry;
                projectEntry.name = it->second.name;
                projectEntry.filePath = it->second.filePath;
                projectEntry.lastOpened = time(NULL);
                
                it = m_userPreferences->recentProjects.erase(it);
                
                m_userPreferences->recentProjects[projectEntry.lastOpened] = projectEntry;
                
                UserPreferencesSerializer preferencesSerializer(m_userPreferences);
                preferencesSerializer.Serialize(m_userPreferences->filePath);
                
                OpenProject(projectEntry.filePath);
                break;
              }
            }
            else
            {
              m_userPreferences->recentProjects.erase(it);
              UserPreferencesSerializer serializer(m_userPreferences);
              serializer.Serialize(m_userPreferences->filePath);
              break;
            }
            
            i++;
          }
          ImGui::EndMenu();
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit", "Cmd + Q"))
        {
          Application::Get().Close();
        }
      });
      
      UI_Utils::AddMenu("Edit", popItemHighlight, [this]() {
        
      });
      
      UI_Utils::AddMenu("View", popItemHighlight, [this]() {

      });
      
      UI_Utils::AddMenu("Debug", popItemHighlight, [this]() {
        
      });
      
      UI_Utils::AddMenu("Help", popItemHighlight, [this]() {

      });
      
      if (menuOpen)
      {
        ImGui::PopStyleColor(2);
      }
    }
    UI::EndMenuBar();
    ImGui::EndGroup();
  }

  void RendererLayer::UI_WindowButtons()
  {
    // Window buttons
    const ImU32 buttonColN = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 0.9f);
    const ImU32 buttonColH = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.2f);
    const ImU32 buttonColP = Kreator_UI::Color::TextDarker;
    const float buttonWidth = 14.0f;
    const float buttonHeight = 14.0f;
    const Window& window = Application::Get().GetWindow();
    
    // Minimize Button
    {
      const int iconHeight = m_iconMinimize->GetHeight();
      const float padY = (buttonHeight - (float)iconHeight) / 2.0f;

      if (ImGui::InvisibleButton("Minimize", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        window.Iconify();
      }
      UI::DrawButtonImage(m_iconMinimize, buttonColN, buttonColH, buttonColP, UI::RectExpanded(UI::GetItemRect(), 0.0f, -padY));
    }

    UI::SameLine();
    // Maximize Button
    {
      bool isMaximized = window.IsMaximized();
      if (ImGui::InvisibleButton("Maximize/Restore", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        (isMaximized) ? window.Restore() : window.Maximize();
      }
      UI::DrawButtonImage(isMaximized ? m_iconRestore : m_iconMaximize, buttonColN, buttonColH, buttonColP);
    }

    UI::SameLine();
    // Close Button
    {
      if (ImGui::InvisibleButton("Close", ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
      {
        Application::Get().Close();
      }
      UI::DrawButtonImage(m_iconClose, UI::Theme::Color::Text, UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.4f), buttonColP);
    }
  }

  void RendererLayer::UI_WelcomePopup()
  {
    if (m_showWelcomePopup)
    {
      ImGui::OpenPopup("Welcome Screen");
      m_showWelcomePopup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 1000, 500 });

    UI::ScopedColor bgCol(ImGuiCol_ChildBg, UI::Theme::Color::BackgroundPopup);
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 15.0f));

    if (ImGui::BeginPopupModal("Welcome Screen", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV ;
      UI::PushID();
      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;

        ImGui::TableSetupColumn("##About/New_Project", 0, 650.0f);
        ImGui::TableSetupColumn("##Recent_Projects", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();
        
        // Content Outliner
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##About/New_Project");
        {
          const int32_t logoSize = 200;
          // Draw Kreator Logo ---------------------------------------------------------------
          {
            const ImVec2 logoOffset(windowPadding.x, windowPadding.y);
            const ImVec2 logoRectStart =
            {
              ImGui::GetItemRectMin().x + (ImGui::GetColumnWidth() / 2 - logoSize / 2),
              ImGui::GetItemRectMin().y
            };
            const ImVec2 logoRectMax =
            {
              logoRectStart.x + logoSize,
              logoRectStart.y + logoSize
            };
            auto* drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(UI::GetTextureID(m_welcomeIcon), logoRectStart, logoRectMax, {0, 0}, {1, 1},
                               IM_COL32(84, 84, 211, 255));
          }
          
          // Welcome Header -------
          {
            UI::SetCursorPosY(logoSize + ImGui::GetCurrentWindow()->WindowPadding.y + 10);
            {
              UI::ScopedFont header(Kreator_UI::GetHugeHeaderFont());
              static std::string welcomeText = "Welcome to IKan-Kreate";
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(welcomeText.c_str()).x / 2);
              ImGui::Text("%s", welcomeText.c_str());
            }
            
            {
              UI::ScopedFont version(Kreator_UI::GetBoldFont());
              static std::string versionText = "Version " + IKanVersion;
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(versionText.c_str()).x / 2);
              ImGui::Text("%s", versionText.c_str());
            }
          }
          ImGui::Separator();
          
          // Buttons
          {
            auto button = [this](const char* title, Ref<Image> icon, const std::string& buttonHelper) {
              auto textSize = ImGui::CalcTextSize(buttonHelper.c_str()).x;;

              const float buttonHeight = 40.0f;
              const float buttonWidth = textSize + buttonHeight + 80;
              
              // Minimize Button
              UI::ShiftCursorX(80);
              if (ImGui::InvisibleButton(title, ImVec2(buttonWidth, buttonHeight), ImGuiButtonFlags_AllowItemOverlap))
              {
                return true;
              }
              
              const ImVec2 logoRectStart = ImGui::GetItemRectMin();
              const ImVec2 logoRectMax =
              {
                logoRectStart.x + buttonHeight,
                logoRectStart.y + buttonHeight
              };
              
              const ImU32 buttonColN = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 0.9f);
              const ImU32 buttonColH = UI::ColorWithMultipliedValue(UI::Theme::Color::Text, 1.2f);
              const ImU32 buttonColP = Kreator_UI::Color::TextDarker;
              UI::DrawButtonImage(icon, buttonColN, buttonColH, buttonColP, ImRect{logoRectStart, logoRectMax});
              
              ImGui::SameLine(140);
              UI::ShiftCursorY(10);
              {
                UI::ScopedFont text(Kreator_UI::GetSemiHeaderFont());
                UI::ScopedColor color(ImGuiCol_Text, IM_COL32(184, 184, 184, 255));

                ImGui::Text("%s", buttonHelper.c_str());
              }
              
              return false;
            };
            
            if (button("New Project", m_newProject, "Create New Kreator Project"))
            {
              m_showCreateNewProjectPopup = true;
              ImGui::CloseCurrentPopup();
            }
            
            if (button("Open Project", m_folder, "Open an exisiting Kreator Project"))
            {
              ImGui::CloseCurrentPopup();
              FolderExplorer::OpenPopup(m_allProjectsPath, &m_showWelcomePopup);
              m_folderExplorerAction = FolderExplorerAction::OpenProject;
            }
          }
          
          UI::ShiftCursorY(35);
          UI::ShiftCursorX(10);
          bool showAgain = m_userPreferences->showWelcomeScreen;
          if (ImGui::Checkbox("##showAgain", &showAgain))
          {
            m_userPreferences->showWelcomeScreen = showAgain;
            UserPreferencesSerializer serializer(m_userPreferences);
            serializer.Serialize(m_userPreferences->filePath);
          }
          ImGui::SameLine();
          ImGui::TextUnformatted("Show this window again when Kreator Launches");

          // Draw side shadow-----------------------------------------------------------------
          ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 0.0f);
          ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
          UI::DrawShadowInner(m_shadowTexture, 12.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f,
                              false, true, false, false);
          ImGui::PopClipRect();
        }
        ImGui::EndChild(); // About/New_Project

        // Directory Content
        ImGui::TableSetColumnIndex(1);
        ImGui::BeginChild("##Recent_Projects");
        {
          UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
          UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 20.0f));
          UI::ScopedFont semiHeader(Kreator_UI::GetSemiHeaderFont());

          m_openProjectPath = "";
          for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
          {
            if (!Utils::FileSystem::Exists(it->second.filePath))
            {
              m_userPreferences->recentProjects.erase(it);
              break;
            }

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding;
            bool open = UI::TreeNode("##Recent_Projects", it->second.name, flags, m_projectIcon);
            if (ImGui::IsItemClicked())
            {
              m_openProjectPath = it->second.filePath;
            }
            
            if(open)
            {
              ImGui::TreePop();
            }
            ImGui::Separator();
          }
          
          if(m_openProjectPath != "")
          {
            OpenProject();
            ImGui::CloseCurrentPopup();
          }
        }
        ImGui::EndChild(); // Recent_Projects

        ImGui::EndTable();
      }
      UI::PopID();
      ImGui::EndPopup();
    }
  }
  void RendererLayer::UI_NewProjectPopup()
  {
    auto boldFont = Kreator_UI::GetBoldFont();
    
    if (m_showCreateNewProjectPopup)
    {
      ImGui::OpenPopup("New Project");
      memset(m_projectNameBuffer, 0, MAX_PROJECT_NAME_LENGTH);
      m_showCreateNewProjectPopup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 200 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
    
    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                               ImGuiWindowFlags_NoTitleBar))
    {
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 200 / 8);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
      
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##new_project_name", "Project Name", m_projectNameBuffer, MAX_PROJECT_NAME_LENGTH);
      
      ImVec2 labelSize = ImGui::CalcTextSize("...", NULL, true);
      auto& style = ImGui::GetStyle();
      ImVec2 buttonSize = ImGui::CalcItemSize(ImVec2(0, 0),
                                              labelSize.x + style.FramePadding.x * 2.0f,
                                              labelSize.y + style.FramePadding.y * 2.0f);

      ImGui::SetNextItemWidth(680 - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1 - buttonSize.x);
      ImGui::InputTextWithHint("##new_project_location", "Project Location",
                               m_projectFilePathBuffer, MAX_PROJECT_FILEPATH_LENGTH, ImGuiInputTextFlags_ReadOnly);
      ImGui::SameLine();
      if (UI::DrawRoundButton("...", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 5))
      {
        ImGui::CloseCurrentPopup();
        FolderExplorer::SelectPopup(m_allProjectsPath, &m_showCreateNewProjectPopup);
        m_folderExplorerAction = FolderExplorerAction::NewPreoject;
      }
      
      ImGui::Separator();
      
      // Buttons
      {
        ImGui::PushFont(boldFont);

        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - 100);
        if ((UI::DrawRoundButton("Create", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
            (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
        {
          std::string fullProjectPath = "";
          if (strlen(m_projectFilePathBuffer) > 0)
          {
            fullProjectPath = std::string(m_projectFilePathBuffer) + "/" + std::string(m_projectNameBuffer);
          }

          CreateProject(fullProjectPath);
          ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if ((UI::DrawRoundButton("Cancel", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
            (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
        {
          m_showWelcomePopup = true;
          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
      }
      
      ImGui::PopStyleVar();

      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
  }
  
  void RendererLayer::UI_FolderExplorer()
  {
    // Folder explorer if clicked ... Button
    auto explorerOutput = FolderExplorer::Explore();
    if (explorerOutput != "")
    {
      switch (m_folderExplorerAction)
      {
        case FolderExplorerAction::NewPreoject:
        {
          memset(m_projectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
          memcpy(m_projectFilePathBuffer, explorerOutput.string().c_str(), MAX_PROJECT_FILEPATH_LENGTH);
          break;
        }
        case FolderExplorerAction::OpenProject:
        {
          if (Utils::String::GetExtensionFromPath(explorerOutput) == ProjectExtension)
          {
            m_openProjectPath = explorerOutput;
            OpenProject();
          }
          else
          {
            FolderExplorer::OpenPopup(explorerOutput.parent_path());
            m_folderExplorerAction = FolderExplorerAction::OpenProject;
          }
          break;
        }
        case FolderExplorerAction::None:
        default:
          IK_ASSERT(false);
          break;
      }
    }
  }
} // namespace Kreator
