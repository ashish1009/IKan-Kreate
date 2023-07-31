//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  // Kretor Resource Path
#define KreatorResourcePath(path) m_clientDirPath / "Resources" / path

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

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientDirPath)
  : Layer("Kreator Renderer"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
  , m_userPreferences(userPreference), m_clientDirPath(clientDirPath)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    m_projectNameBuffer = iknew char[MAX_PROJECT_NAME_LENGTH];
    m_projectFilePathBuffer = iknew char[MAX_PROJECT_FILEPATH_LENGTH];
    
    m_templateProjectDir = m_clientDirPath / "Resources/TemplateProject";
    
    // Set the Application Icon
    m_applicationIcon = Image::Create(KreatorResourcePath("Textures/Logo/IKan.png"));
    
    // Window Icons
    m_iconClose = Image::Create(KreatorResourcePath("Textures/Icons/Close.png"));
    m_iconMinimize = Image::Create(KreatorResourcePath("Textures/Icons/Minimize.png"));
    m_iconMaximize = Image::Create(KreatorResourcePath("Textures/Icons/Maximize.png"));
    m_iconRestore = Image::Create(KreatorResourcePath("Textures/Icons/Restore.png"));
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
    Kreator_UI::SetDarkTheme();
    
    UI::Font regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    UI::Font boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    UI::Font italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    UI::Font sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};    
    UI::Theme::ChangeFont({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont});

    // Open or Create Project
    if (Utils::FileSystem::Exists(m_userPreferences->startupProject))
    {
      OpenProject(m_userPreferences->startupProject);
    }
    else
    {
      auto projName = Utils::String::GetFileNameFromPath(m_userPreferences->startupProject);
      auto projDir = Utils::String::GetDirectoryFromPath(m_userPreferences->startupProject);
      
      memcpy(m_projectNameBuffer, projName.c_str(), projName.size());
      CreateProject(projDir);
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
    
    UI_StartMainWindowDocking();
    
    UI_Viewport();
        
    UI_EndMainWindowDocking();
  }
  
  void RendererLayer::UpdateWindowTitle(const std::string& sceneName)
  {
    const auto& caps = Renderer::Capabilities::Get();
    const std::string title = fmt::format("{0} ({1}) - Kreator - {2} ({3}) Renderer: {4}",
                                          sceneName, Project::GetActive()->GetConfig().name,
                                          caps.vendor, caps.version,
                                          caps.renderer);
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
  }
  
  void RendererLayer::OpenProject(const std::string &filepath)
  {
    IK_LOG_TRACE("Kreator Layer", "Opening Project {0}", filepath.c_str());

    // Reset Project Name and directory
    memset(m_projectNameBuffer, 0, MAX_PROJECT_NAME_LENGTH);
    memset(m_projectFilePathBuffer, 0, MAX_PROJECT_FILEPATH_LENGTH);
    
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
  }
  
  void RendererLayer::OpenProject()
  {
    IK_ASSERT(false);
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
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, Kreator_UI::Color::Muted, 2.0f);
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
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, Kreator_UI::Color::Muted, 2.0f);
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

} // namespace Kreator
