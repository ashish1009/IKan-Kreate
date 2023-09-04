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
  // Kretor Resource Path
#define KreatorResourcePath(path) Utils::FileSystem::Absolute(m_clientResourcePath / path)

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

  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }
  
  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_userPreferences(userPreference), m_clientResourcePath(clientResourcePath),
  m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
    
    // Save All Project Path
#ifdef DEBUG
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#else
    m_allProjectsPath = m_clientResourcePath / "../Projects";
#endif

    // Save the default project path
    auto fullAllProjectPath =  Utils::FileSystem::IKanAbsolute(m_allProjectsPath);
    m_projectFilePathBuffer.MemCpy(fullAllProjectPath.data(), 0, fullAllProjectPath.size());

    // Save the template project dir
    m_templateProjectDir = m_clientResourcePath / "TemplateProject";
    
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
 }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
    
    // Decorate the Theme ------------------------------------------------------------------------------------------
    // Set all the required Fonts
    UI::Font regularFontFilePath = {KreatorResourcePath("Fonts/Opensans/Regular.ttf"), 14};
    UI::Font boldFontFilePath = {KreatorResourcePath("Fonts/Opensans/ExtraBold.ttf"), 14};
    UI::Font italicFontFilePath = {KreatorResourcePath("Fonts/Opensans/Italic.ttf"), 14};
    UI::Font sameWidthFont = {KreatorResourcePath("Fonts/HfMonorita/Regular.ttf"), 10};
    UI::Font hugeheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 40};
    UI::Font semiheader = {KreatorResourcePath("Fonts/Opensans/Bold.ttf"), 18};
    UI::Theme::ChangeFont({regularFontFilePath, boldFontFilePath, italicFontFilePath, sameWidthFont, hugeheader, semiheader});
    
    // Set the Theme of ImGui as user preference
    switch (m_userPreferences->theme)
    {
      case UserPreferences::Theme::KreatorDark:
        Kreator_UI::SetDarkTheme();
        break;
        
      default:
      case UserPreferences::Theme::Default:
        break;
    }
    
    // Open or Create Project ---------------------------------------------------------------------------------------
    if (Utils::FileSystem::Exists(m_userPreferences->startupProject))
    {
      if (m_userPreferences->showWelcomeScreen)
      {
        m_showWelcomePopup = true;
      }
      else
      {
        IK_ASSERT(false, "Open the Project");
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
    
    m_editorCamera.SetActive(m_allowViewportCameraEvents or Input::GetCursorMode() == CursorMode::Locked);
    m_editorCamera.OnUpdate(ts);

    m_viewport.UpdateMousePos();

    Renderer::Clear({0.1f, 0.1f, 0.14f, 1.0f});
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnKeyPressedEvent));
    dispatcher.Dispatch<MouseButtonPressedEvent>(IK_BIND_EVENT_FN(RendererLayer::OnMouseButtonPressed));

    m_panels.OnEvent(event);
    
    if (m_viewport.panelMouseHover)
    {
      m_editorCamera.OnEvent(event);
    }
  }
  
  bool RendererLayer::OnKeyPressedEvent(KeyPressedEvent& e)
  {
    return false;
  }
  
  void RendererLayer::UpdateViewportSize()
  {
    m_editorCamera.SetViewportSize(m_viewport.width, m_viewport.height);
  }
  
  bool RendererLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
  {
    return false;
  }
  
  void RendererLayer::OnImGuiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImGuiRender");
    
    // Should be above all scene GUI
    UI_WelcomePopup();
    UI_NewProjectPopup();
    UI_FolderExplorer();
    
    m_panels.OnImGuiRender();
  }
  
  void RendererLayer::CreateProject(const std::filesystem::path &projectDir)
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Project at {0} ", projectDir.string().c_str());

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
    Utils::FileSystem::CreateDirectory(projectDir / "Assets" / "Meshes" / "Source");
    
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
    
    m_panels.OnProjectChanged(project);
    
    // Reset cameras
    m_editorCamera = EditorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
    
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
    SaveProject();
  }
  
  void RendererLayer::CloseProject(bool unloadProject)
  {

  }
  
  void RendererLayer::SaveProject()
  {

  }
  
  // UI APIS ---------------------------------------------------------------------------------------------------------
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

    UI::ScopedColor bgCol(ImGuiCol_ChildBg, IKan::UI::Theme::Color::BackgroundPopup);
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 15.0f));
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 10);

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

        // About/New_Project -----------------------------------------------------
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##About/New_Project");
        {
          const int32_t logoSize = 200;

          // Draw Kreator Logo -----
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
                               IM_COL32(255, 255, 255, 255));
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
              FolderExplorer::OpenPopup("Open Project", m_allProjectsPath, &m_showWelcomePopup);
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

        ImGui::EndTable();
      }
      
      UI::PopID();
      ImGui::EndPopup();
    }
  }
  
  void RendererLayer::UI_NewProjectPopup()
  {
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 0);

    if (m_showCreateNewProjectPopup)
    {
      ImGui::OpenPopup("New Project");
      m_projectNameBuffer.Memset(0);
      m_showCreateNewProjectPopup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 0 });
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));

    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoTitleBar))
    {
      // Title of Popupt --------------------------------------------------
      {
        UI::ScopedColor muted(ImGuiCol_Text, IM_COL32(155, 155, 155, 255));

        static const std::string title = "New Project";
        static const auto titleSize = ImGui::CalcTextSize(title.c_str());

        UI::ScopedFont version(Kreator_UI::GetSemiHeaderFont());
        UI::SetCursorPosY(10);
        UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 -  titleSize.x / 2);
        ImGui::Text(title.c_str());
      }
      
      UI::ShiftCursorY(-20);
      ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 200 / 8);
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
      
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##new_project_name", "Project Name", m_projectNameBuffer.Data(), m_projectNameBuffer.Size());
      
      ImVec2 labelSize = ImGui::CalcTextSize("...", NULL, true);
      auto& style = ImGui::GetStyle();
      ImVec2 buttonSize = ImGui::CalcItemSize(ImVec2(0, 0),
                                              labelSize.x + style.FramePadding.x * 2.0f,
                                              labelSize.y + style.FramePadding.y * 2.0f);

      ImGui::SetNextItemWidth(680 - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1 - buttonSize.x);
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Theme::Color::Muted);
        ImGui::InputTextWithHint("##new_project_location", "Project Location",
                                 m_projectFilePathBuffer.Data(), m_projectFilePathBuffer.Size(), ImGuiInputTextFlags_ReadOnly);
      }
      ImGui::SameLine();
      if (UI::DrawRoundButton("...", Kreator_UI::ColorVec3FromU32(UI::Theme::Color::BackgroundDark), 0))
      {
        ImGui::CloseCurrentPopup();
        FolderExplorer::SelectPopup("Select Project Directory", m_allProjectsPath, &m_showCreateNewProjectPopup);
        m_folderExplorerAction = FolderExplorerAction::NewPreoject;
      }
      
      // Buttons
      {
        auto boldFont = Kreator_UI::GetBoldFont();
        ImGui::PushFont(boldFont);
        
        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - 90);
        if ((UI::DrawRoundButton("Create", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 20)) or
            (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
        {
          std::string fullProjectPath = "";
          if (strlen(m_projectFilePathBuffer.Data()) > 0)
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
//          if (!Project::GetActive())
//          {
//            m_showWelcomePopup = true;
//          }
//          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
      }

      ImGui::PopStyleVar();
      ImGui::EndPopup();
    }
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
          break;
        }
        case FolderExplorerAction::OpenProject:
        {
          break;
        }
        case FolderExplorerAction::SaveScene:
        {
          break;
        }
        case FolderExplorerAction::OpenScene:
        {
          break;
        }
          
        case FolderExplorerAction::None:
        default:
          IK_ASSERT(false);
          break;
      }
    }
  }
  
  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
} // namespace Kreator
