//
//  KreatorLayerUI.cpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#include "KreatorLayer.hpp"
#include "Editor/FolderExplorer.hpp"
#include "Editor/EntityUtils.hpp"

namespace Kreator
{
  // Kreator UI utils
  namespace UI_Utils
  {
    // Hovered Item Color
    static const ImU32 s_hoveredColor = UI::Color::HoveredItem;
    
    // Function to push Dark color on active
    static const auto pushDarkTextIfActive = [](const char* menuName)
    {
      if (ImGui::IsPopupOpen(menuName))
      {
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::BackgroundDark);
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
  } // namespace Utils
  
  void KreatorLayer::UI_WelcomePopup()
  {
    IK_PERFORMANCE("KreatorLayer::UI_WelcomePopup");
    if (m_showWelcomePopup)
    {
      ImGui::OpenPopup("Welcome Screen");
      m_showWelcomePopup = false;
    }

    // Keep the window at center
    UI::SetNextWindowAtCenterWithSize({1000, 450});

    UI::ScopedColor bgCol(ImGuiCol_ChildBg, UI::Color::BackgroundPopup);
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 10.0f));
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 10);
    UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 2.0f));

    if (ImGui::BeginPopupModal("Welcome Screen", nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      UI::PushID();

      ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        const ImVec2 windowPadding = ImGui::GetCurrentWindow()->WindowPadding;
        
        ImGui::TableSetupColumn("##About", 0, 650.0f);
        ImGui::TableSetupColumn("##Recent_Projects", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();
        
        // About/New_Project -----------------------------------------------------
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##About");
        {
          static constexpr int32_t logoSize {200};
          
          // Draw Kreator Logo -----
          {
            const ImVec2 logoOffset(windowPadding.x, windowPadding.y);
            const ImVec2 logoRectStart
            {
              ImGui::GetItemRectMin().x + (ImGui::GetColumnWidth() / 2 - logoSize / 2),
              ImGui::GetItemRectMin().y
            };
            const ImVec2 logoRectMax
            {
              logoRectStart.x + logoSize,
              logoRectStart.y + logoSize
            };
            auto* drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(UI::GetTextureID(m_welcomeIcon), logoRectStart, logoRectMax, {0, 1}, {1, 0});
          }
          
          // Welcome Header -------
          {
            UI::SetCursorPosY(logoSize + ImGui::GetCurrentWindow()->WindowPadding.y + 10);
            {
              UI::ScopedFont header(UI::GetHugeHeaderFont());
              static std::string welcomeText = "Welcome to IKan-Kreate";
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(welcomeText.c_str()).x / 2);
              ImGui::Text("%s", welcomeText.c_str());
            }
            
            {
              UI::ScopedFont version(UI::GetBoldFont());
              static std::string versionText = "Version v" + IKanVersion + "." + KreatorVersion;
              UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(versionText.c_str()).x / 2);
              ImGui::Text("%s", versionText.c_str());
            }
          }
          ImGui::Separator();

          // Buttons
          {
            auto button = [this](const char* title, Ref<Texture> icon, const std::string& buttonHelper) {
              auto textSize = ImGui::CalcTextSize(buttonHelper.c_str()).x;;
              
              static const float buttonHeight = 40.0f;
              static const float buttonWidth = textSize + buttonHeight + 80;
              
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
              
              static const ImU32 buttonColN = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 0.9f);
              static const ImU32 buttonColH = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 1.2f);
              static const ImU32 buttonColP = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 1.0f);
              UI::DrawButtonImage(icon, buttonColN, buttonColH, buttonColP, ImRect{logoRectStart, logoRectMax});
              
              ImGui::SameLine(140);
              UI::ShiftCursorY(10);
              {
                UI::ScopedFont text(UI::GetSemiHeaderFont());
                UI::ScopedColor color(ImGuiCol_Text, UI::Color::TextDarker);
                ImGui::Text("%s", buttonHelper.c_str());
              }
              return false;
            };
            
            if (button("New Project", m_newProjectIcon, "Create New Kreator Project"))
            {
              ImGui::CloseCurrentPopup();
              m_showCreateNewProjectPopup = true;
            }
            
            if (button("Open Project", m_folderIcon, "Open an exisiting Kreator Project"))
            {
              ImGui::CloseCurrentPopup();
              FolderExplorer::Open(ProjectExtension, "", &m_showWelcomePopup);
              m_folderExplorerAction = FolderExplorerAction::OpenProject;
            }
          }

          UI::ShiftCursorY(15);
          UI::ShiftCursorX(10);
          if (ImGui::Checkbox("##showAgain", &m_userPreferences->showWelcomeScreen))
          {
            UserPreferencesSerializer serializer(m_userPreferences);
            serializer.Serialize(m_userPreferences->filePath);
          }
          
          ImGui::SameLine();
          ImGui::TextUnformatted("Show this window again when Kreator Launches");

          // Draw side shadow-----------------------------------------------------------------
          ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 0.0f);
          ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
          UI::DrawShadowInner(m_shadowTexture, 12.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f, false, true, false, false);
          ImGui::PopClipRect();
        } // Begin child About
        ImGui::EndChild(); // About/New_Project
        
        // Recent Project
        ImGui::TableSetColumnIndex(1);
        ImGui::BeginChild("##Recent_Projects");
        {
          // Title of Popup --------------------------------------------------
          {
            UI::ScopedColor muted(ImGuiCol_Text, UI::Color::TextDarker);
            
            static const std::string title = "Recent Projects";
            static const auto titleSize = ImGui::CalcTextSize(title.c_str());
            
            UI::ScopedFont version(UI::GetSemiHeaderFont());
            UI::SetCursorPosY(10);
            UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 - titleSize.x / 1.4);
            ImGui::Text(title.c_str());
            ImGui::Separator();
          }
          
          // Recent Project --------------------------------------------------
          {
            UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 20.0f));
            UI::ScopedFont semiHeader(UI::GetSemiHeaderFont());
            
            m_openProjectPath = "";
            for (auto it = m_userPreferences->recentProjects.begin(); it != m_userPreferences->recentProjects.end(); it++)
            {
              if (!std::filesystem::exists(it->second.filePath))
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
            }
            
            if(m_openProjectPath != "")
            {
              OpenProject();
              ImGui::CloseCurrentPopup();
            }
          }
          
          // Theme Selection --------------------------------------------------
          {
            UI::SetCursorPosY(ImGui::GetWindowHeight() - 30);
            int32_t theme = (int32_t)m_userPreferences->theme;
            if (UI::PropertyDropdownNoLabel("Theme", {"Blue", "Dark", "Light"}, 3, &theme))
            {
              m_userPreferences->theme = static_cast<UserPreferences::Theme>(theme);
              UserPreferencesSerializer serializer(m_userPreferences);
              serializer.Serialize(m_userPreferences->filePath);
              
              UI::SetThemeColors(m_userPreferences->theme);
            }
          }
        }
        ImGui::EndChild(); // Recent project
        ImGui::EndTable();
      } // Begin Table
      UI::PopID();
      ImGui::EndPopup();
    } // Begin Popup Model
  }
  
  void KreatorLayer::UI_NewProjectPopup()
  {
    IK_PERFORMANCE("KreatorLayer::UI_NewProjectPopup");
    if (m_showCreateNewProjectPopup)
    {
      ImGui::OpenPopup("New Project");
      m_projectNameBuffer.Memset(0);
      m_showCreateNewProjectPopup = false;
    }
    
    UI::SetNextWindowAtCenterWithSize({700, 0});
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
    
    if (ImGui::BeginPopupModal("New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar))
    {
      // Title of Popup --------------------------------------------------
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Color::TextDarker);
        
        static const std::string title = "New Project";
        static const auto titleSize = ImGui::CalcTextSize(title.c_str());
        
        UI::ScopedFont version(UI::GetSemiHeaderFont());
        UI::SetCursorPosY(10);
        UI::SetCursorPosX(ImGui::GetColumnWidth() / 2 -  titleSize.x / 2);
        ImGui::Text(title.c_str());
      }
      
      // Project name and directory
      {
        UI::ShiftCursorY(-20);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 200 / 8);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 7));
        
        ImGui::SetNextItemWidth(-1);
        if (ImGui::IsItemHovered())
        {
          ImGui::SetKeyboardFocusHere();
        }
        ImGui::InputTextWithHint("##new_project_name", "Project Name", m_projectNameBuffer.Data(), m_projectNameBuffer.Size());
        
        ImVec2 labelSize = ImGui::CalcTextSize("...", NULL, true);
        auto& style = ImGui::GetStyle();
        ImVec2 buttonSize = ImGui::CalcItemSize(ImVec2(0, 0), labelSize.x + style.FramePadding.x * 2.0f, labelSize.y + style.FramePadding.y * 2.0f);
        
        ImGui::SetNextItemWidth(680 - style.FramePadding.x * 2.0f - style.ItemInnerSpacing.x - 1 - buttonSize.x);
        {
          UI::ScopedColor muted(ImGuiCol_Text, UI::Color::Muted);
          ImGui::InputTextWithHint("##new_project_location", "Project Location", m_projectFilePathBuffer.Data(), m_projectFilePathBuffer.Size(), ImGuiInputTextFlags_ReadOnly);
        }
        
        ImGui::SameLine();
        if (UI::DrawRoundButton("...", UI::Color::BackgroundDark, 5))
        {
          ImGui::CloseCurrentPopup();
          FolderExplorer::Select(&m_showCreateNewProjectPopup);
          m_folderExplorerAction = FolderExplorerAction::NewPreoject;
        }
      }

      // Buttons
      {
        UI::PushID();
        ImGui::SetItemDefaultFocus();
        ImGui::PushFont(UI::GetBoldFont());
        
        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - 90);
        if (UI::DrawRoundButton("Create", UI::Color::NiceThemeHighlight, 10) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
        {
          if (!m_projectNameBuffer.Empty())
          {
            std::filesystem::path fullProjectPath {};
            if (strlen(m_projectFilePathBuffer.Data()) > 0)
            {
              fullProjectPath = std::filesystem::path(m_projectFilePathBuffer) / std::filesystem::path(m_projectNameBuffer);
            }
            
            CreateProject(fullProjectPath);
            ImGui::CloseCurrentPopup();
          }
        }
        
        ImGui::SameLine();
        if (UI::DrawRoundButton("Cancel", UI::Color::Muted, 10) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
        {
          if (!Project::GetActive())
          {
            m_showWelcomePopup = true;
          }
          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
        UI::PopID();
      }

      ImGui::PopStyleVar();
      ImGui::EndPopup();
    } // Begin Popup Model
  }
  
  void KreatorLayer::UI_FolderExplorer()
  {
    IK_PERFORMANCE("KreatorLayer::UI_FolderExplorer");
    std::filesystem::path explorerOutput = FolderExplorer::Explore();
    RETURN_IF(explorerOutput == "");
    
    switch (m_folderExplorerAction)
    {
      case FolderExplorerAction::NewPreoject:
      {
        m_projectFilePathBuffer.MemCpy(explorerOutput.string().c_str(), 0, 255);
        break;
      }
      case FolderExplorerAction::OpenProject:
      {
        if (explorerOutput.extension() == ProjectExtension)
        {
          m_openProjectPath = explorerOutput;
          OpenProject();
        }
        else
        {
          IK_ASSERT(false, "Implement later");
        }
        break;
      }
      case FolderExplorerAction::SaveScene:
      {
        m_sceneFilePath = explorerOutput;
        m_sceneFilePath += SceneExtension;
        
        SaveScene();

        break;
      }
      case FolderExplorerAction::OpenScene:
      {
        if (explorerOutput.extension() == SceneExtension)
        {
          OpenScene(explorerOutput);
        }
        else
        {
          IK_ASSERT(false, "Implement later");
        }
        break;
      }


      case FolderExplorerAction::None:
      default:
        IK_ASSERT(false);
        break;
    }
  }
  
  void KreatorLayer::UI_StartMainWindowDocking()
  {
    IK_PERFORMANCE("KreatorLayer::UI_StartMainWindowDocking");
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
    
    if (m_sceneState != SceneState::Play)
    {
      // Render the title if original title bar is hidden
      if (Application::Get().GetSpecification().windowSpecificaion.hideTitleBar)
      {
        float titlebarHeight = UI_DrawTitlebar();
        UI::SetCursorPosY(titlebarHeight + ImGui::GetCurrentWindow()->WindowPadding.y);
      }
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
  
  void KreatorLayer::UI_EndMainWindowDocking()
  {
    IK_PERFORMANCE("KreatorLayer::UI_EndMainWindowDocking");
    ImGui::End();
  }
  
  float KreatorLayer::UI_DrawTitlebar()
  {
    IK_PERFORMANCE("KreatorLayer::UI_DrawTitlebar");
    static const float titleBarHeight = 40.0f;
    
    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + titleBarHeight
    };
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Titlebar);
    
    // Drag and Control the window with user title bar ---------------------------------
    UI_TitlebarDragArea(titleBarHeight);

    // Draw Kreator Logo ---------------------------------------------------------------
    static const int32_t logoWidth = titleBarHeight - 10;
    static const int32_t logoHeight = titleBarHeight - 10;
    const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x,
      ImGui::GetItemRectMin().y
    };
    const ImVec2 logoRectMax =
    {
      logoRectStart.x + logoWidth,
      logoRectStart.y + logoHeight
    };
    drawList->AddImage(UI::GetTextureID(m_applicationIcon), logoRectStart, logoRectMax, {0, 1}, {1, 0});

    // Draw the Menu Tab in Title bar --------------------------------------------------
    ImGui::SetItemAllowOverlap();
    
    static const float logoOffsetX = (6.0f * 2.0f) + 41.0f;
    UI::SetCursorPos(ImVec2(logoOffsetX, 4.0f));
    
    UI_MenuBar();

    // Round Bar -------------------------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() / 4);
    UI::SetCursorPosY(titleBarHeight / 3);
    
    const ImVec2 roundBarMin = ImGui::GetCursorScreenPos();
    const ImVec2 roundBarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() / 2,
      ImGui::GetCursorScreenPos().y + 23
    };
    drawList->AddRectFilled(roundBarMin, roundBarMax, UI::Color::TitleBarDark, 10);
    float roundBarRight = (3 * ImGui::GetWindowWidth()) / 4;
    UI::ShiftCursorY(5);

    const float underlineThickness = 1.0f;
    // Project name --------------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, UI::Color::TextDarker);
      UI::ShiftCursorX(10);
      const std::string title = Project::GetActive()->GetConfig().name;
      {
        UI::ScopedFont boldFont(UI::GetBoldFont());
        ImGui::Text(title.c_str());
      }
      UI::SetTooltip("Current project (" + Project::GetActive()->GetConfig().projectFileName + ")");
      
      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);
      // Make the Max + thickness of rectange as min of verticle line
      itemRect.Min.x = itemRect.Max.x + underlineThickness;
      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Color::Muted, 2.0f);
    }
    
    // Current Scene name ---------------------------------------------------------------
    {
      UI::ScopedColor textColor(ImGuiCol_Text, UI::Color::Text);
      UI::SameLine();
      
      const std::string sceneName = m_currentScene->GetName();
      UI::SetCursorPosX(roundBarRight - ImGui::CalcTextSize(sceneName.c_str()).x - 20);
      {
        if (m_sceneFilePath == "")
        {
          ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::Warning);
        }
        UI::ScopedFont boldFont(UI::GetBoldFont());
        ImGui::Text(sceneName.c_str());
        if (m_sceneFilePath == "")
        {
          ImGui::PopStyleColor();
        }
      }
      if (m_sceneFilePath == "")
      {
        UI::SetTooltip("Unsaved Scene");
      }
      else
      {
        UI::SetTooltip("Current scene (" + m_sceneFilePath.string() + ")");
      }
      
      // Get the Project Name rectangle (Expanded by 10 to have good space)
      ImRect itemRect = UI::RectExpanded(UI::GetItemRect(), 10.0f, 2.0f);
      
      // Make the new Min + Thickness of rectange as new Max of verticle line
      itemRect.Max.x = itemRect.Min.x + underlineThickness;
      
      drawList->AddRectFilled(itemRect.Min, itemRect.Max, UI::Color::Muted, 2.0f);
    }
    
    // Render the Window Buttons -------------------------------------------------------
    UI::SetCursorPosX(ImGui::GetWindowWidth() - 78);
    UI::SetCursorPosY(20.0f);
    UI_WindowButtons();

    return titleBarHeight;
  }
  
  void KreatorLayer::UI_TitlebarDragArea(float titlebarHeight)
  {
    static float moveOffsetX;
    static float moveOffsetY;
    const float titleBarWidth = ImGui::GetContentRegionAvail().x;
    
    auto* rootWindow = ImGui::GetCurrentWindow()->RootWindow;
    const float windowWidth = (int32_t)rootWindow->RootWindow->Size.x;
    
    if (ImGui::InvisibleButton("##titleBarDragZone", ImVec2(titleBarWidth, titlebarHeight * 2), ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_AllowItemOverlap))
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
  
  void KreatorLayer::UI_MenuBar()
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
        const ImU32 colActive = UI::ColorWithSaturation(UI::Color::Accent, 0.5f);
        ImGui::PushStyleColor(ImGuiCol_Header, colActive);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, colActive);
      }
      
      // Function to Pop the highlight Color
      static auto popItemHighlight = [&menuOpen]
      {
        if (menuOpen)
        {
          ImGui::PopStyleColor(2);
          menuOpen = false;
        }
      };
      
      // Menu Items
      UI_Utils::AddMenu("File", popItemHighlight, [this]() {

      });
      
      UI_Utils::AddMenu("Edit", popItemHighlight, [this]() {
        
      });
      
      UI_Utils::AddMenu("View", popItemHighlight, [this]() {
        for (auto& [id, panelData] : m_panels.GetPanels())
        {
          ImGui::MenuItem(panelData.name, nullptr, &panelData.isOpen);
        }
        ImGui::Separator();
        ImGui::MenuItem("Statistic Panel", nullptr, &m_showStatisticsPanel);
        ImGui::MenuItem("Scene Setting Panel", nullptr, &m_showSceneSettings);
      });
      
      UI_Utils::AddMenu("Debug", popItemHighlight, [this]() {
        ImGui::MenuItem("Show System Info", nullptr, &m_renderSystemInfo);
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
  
  void KreatorLayer::UI_WindowButtons()
  {
    // Window buttons
    static const ImU32 buttonColN = UI::ColorWithMultipliedValue(UI::Color::Text, 0.9f);
    static const ImU32 buttonColH = UI::ColorWithMultipliedValue(UI::Color::Text, 1.2f);
    static const ImU32 buttonColP = UI::Color::TextDarker;
    static const float buttonWidth = 14.0f;
    static const float buttonHeight = 14.0f;
    static const Window& window = Application::Get().GetWindow();
    
    // Minimize Button
    {
      static const int iconHeight = m_iconMinimize->GetHeight();
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
      UI::DrawButtonImage(m_iconClose, UI::Color::Text, UI::ColorWithMultipliedValue(UI::Color::Text, 1.4f), buttonColP);
    }
  }
  void KreatorLayer::UI_StatisticsPanel()
  {
    IK_PERFORMANCE("KreatorLayer::UI_StatisticsPanel");
    RETURN_IF (!m_showStatisticsPanel)
    if (ImGui::Begin("Statistics", &m_showStatisticsPanel))
    {
      UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
      UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 5);

      ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_None;
      if (ImGui::BeginTabBar("StatisticsTabs", tabBarFlags))
      {
        if (ImGui::BeginTabItem("Renderer"))
        {
          UI::ScopedDisable disable;
          auto& caps = RendererCapabilities::Get();
          UI::BeginPropertyGrid(2, 1, 1);
          UI::Property("Vendor", caps.vendor);
          UI::Property("Renderer", caps.renderer);
          UI::Property("Version", caps.version);
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
            const auto& perFrameData = PerformanceProfiler::Get()->GetPerFrameData();
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
  
  void KreatorLayer::UI_Viewport()
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
    
    // Set Viewport of Dummy Data
    UpdateViewportSize();
    
    // Render viewport image
    UI::Image(m_viewportRenderer.GetFinalImage(), viewportSize);

    // Right click Menu --------------------------------------------------------------------------------------
    {
      UI::ScopedStyle windowPaadding(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
      UI::ScopedStyle itemSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
      if (ImGui::BeginPopupContextWindow(0))
      {
#if 0
        if (m_selectionContext.size())
        {
          ECS_Utils::DrawCreateEntityMenu(m_currentScene, m_selectionContext.at(0).entity);
        }
        else
#endif
        {
          Entity newEntity = ECS_Utils::DrawCreateEntityMenu(m_currentScene, {});
          if (newEntity)
          {
            ClearSelectedEntity();
            m_selectionContext.push_back({newEntity, 0.0f});
            SetSelectedEntity(newEntity);
          }
        }
        ImGui::EndPopup(); // Right click
      }
    }
    
    if (m_sceneState != SceneState::Play)
    {
      UI_GuizmoToolbar();
      UI_UpdateGuizmo();
    }

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
  
  void KreatorLayer::UI_NewScenePopup()
  {
    static char s_newSceneName[128];
    if (m_showNewScenePopup)
    {
      ImGui::OpenPopup("New Scene");
      memset(s_newSceneName, 0, 128);
      m_showNewScenePopup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 400,0 });
    if (ImGui::BeginPopupModal("New Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
      ImGui::BeginVertical("NewSceneVertical");
      ImGui::SetNextItemWidth(-1);
      ImGui::InputTextWithHint("##scene_name_input", "Scene Name", s_newSceneName, 128);
      
      ImGui::BeginHorizontal("NewSceneHorizontal");
      
      UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("Create").x * 2);
      if (UI::DrawRoundButton("Create", UI::ColorVec3FromU32(UI::Color::NiceThemeHighlight), 10) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
      {
        if ((s_newSceneName[0] != '\0'))
        {
          NewScene(s_newSceneName);
          ImGui::CloseCurrentPopup();
        }
      }
      
      if (UI::DrawRoundButton("Cancel", UI::ColorVec3FromU32(UI::Color::NiceThemeHighlight), 10) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
      {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndHorizontal();
      ImGui::EndVertical();
      ImGui::EndPopup();
    }
  }
  
  void KreatorLayer::UI_SceneSettings()
  {
    RETURN_IF (!m_showSceneSettings)
    
    ImGui::Begin("Scene Settings", &m_showSceneSettings);
    
    // Environment ------------------------------------------
    ImGui::PushID("Environment");
    if (UI::PropertyGridHeader("Environment", true, 4, 5))
    {
      UI::BeginPropertyGrid(2, 1, 5);
      auto& directonLight = SceneRenderer::GetDirectionLight();
      bool useDirectionLight = directonLight.enable;
      if (UI::Property("Direction Light", useDirectionLight))
      {
        directonLight.enable = useDirectionLight;
      }
      if (directonLight.enable)
      {
        UI::PropertyColor("Radiance ", directonLight.color);
        UI::Property("Direction ", directonLight.direction, 0.05, -1.0f, 1.0f);
        UI::Property("Intensity ", directonLight.intensity);
      }
      UI::EndPropertyGrid();
      UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
    
    ImGui::End();
  }
  
  void KreatorLayer::UI_UpdateGuizmo()
  {
    RETURN_IF (Input::IsKeyPressed(IKan::Key::LeftControl))
    
    if (m_gizmoType != -1 and m_selectionContext.size() and m_currentScene != m_runtimeScene)
    {
      float rw = (float)ImGui::GetWindowWidth();
      float rh = (float)ImGui::GetWindowHeight();
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();
      ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
      
      bool snap = Input::IsKeyPressed(Key::LeftShift);
      float snapValue = GetSnapValue();
      float snapValues[3] = { snapValue, snapValue, snapValue };
      
      if (Input::IsKeyPressed(Key::LeftAlt))
      {
        m_gizmoMode = 1;
      }
      else
      {
        m_gizmoMode = 0;
      }
      
      auto& selection = m_selectionContext[0];
      TransformComponent& entityTransform_1 = selection.entity.GetTransform();
      
      glm::mat4 transform = entityTransform_1.Transform();
      ImGuizmo::Manipulate(glm::value_ptr(m_editorCamera.GetViewMatrix()),
                           glm::value_ptr(m_editorCamera.GetUnReversedProjectionMatrix()),
                           (ImGuizmo::OPERATION)m_gizmoType,
                           (ImGuizmo::MODE)m_gizmoMode,
                           glm::value_ptr(transform),
                           nullptr,
                           snap ? snapValues : nullptr);
      
      glm::vec3 translation, rotation, scale;
      Utils::Math::DecomposeTransform(transform, translation, rotation, scale);
      
      glm::vec3 deltaPosition = glm::vec3(0.0f);
      if (m_gizmoType == ImGuizmo::OPERATION::TRANSLATE)
      {
        deltaPosition = translation - entityTransform_1.Position();
      }
      glm::vec3 deltaRotation = glm::vec3(0.0f);
      if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
      {
        deltaRotation = rotation - entityTransform_1.Rotation();
      }
      glm::vec3 deltaScale = glm::vec3(0.0f);
      if (m_gizmoType == ImGuizmo::OPERATION::SCALE)
      {
        deltaScale = scale - entityTransform_1.Scale();
      }
      
      for (auto& selection : m_selectionContext)
      {
        if (ImGuizmo::IsUsing())
        {
          ECS_Utils::UpdateChildrenTransform(m_currentScene, selection.entity, deltaPosition, deltaScale, deltaRotation);
        } // If Guizmo using
      } // for each selection context
    } // if valid selection and guizmo
  }
  
  void KreatorLayer::UI_GuizmoToolbar()
  {
    UI::ScopedStyle disableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    UI::ScopedStyle disableWindowBorder(ImGuiStyleVar_WindowBorderSize, 0.0f);
    UI::ScopedStyle windowRounding(ImGuiStyleVar_WindowRounding, 4.0f);
    UI::ScopedStyle disablePadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    auto viewportStart = ImGui::GetItemRectMin();
    
    const float buttonSize = 18.0f;
    const float edgeOffset = 4.0f;
    const float windowHeight = 32.0f; // annoying limitation of ImGui, window can't be smaller than 32 pixels
    const float numberOfButtons = 4.0f;
    const float windowWidth = edgeOffset * 6.0f + buttonSize * numberOfButtons + edgeOffset * (numberOfButtons - 1.0f) * 2.0f;
    
    ImGui::SetNextWindowPos(ImVec2(viewportStart.x + 14, viewportStart.y + edgeOffset));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowBgAlpha(0.0f);
    
    ImGui::Begin("##viewport_tools", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
    
    // A hack to make icon panel appear smaller than minimum allowed by ImGui size
    // Filling the background for the desired 26px height
    const float desiredHeight = 26.0f;
    ImRect background = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, -(windowHeight - desiredHeight) / 2.0f);
    ImGui::GetWindowDrawList()->AddRectFilled(background.Min, background.Max, IM_COL32(15, 15, 15, 127), 4.0f);
    
    ImGui::BeginVertical("##gizmosV", ImGui::GetContentRegionAvail());
    ImGui::Spring();
    ImGui::BeginHorizontal("##gizmosH", { ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y });
    ImGui::Spring();
    
    {
      UI::ScopedStyle enableSpacing(ImGuiStyleVar_ItemSpacing, ImVec2(edgeOffset * 2.0f, 0));
      
      const ImColor c_SelectedGizmoButtonColor = UI::Color::Accent;
      const ImColor c_UnselectedGizmoButtonColor = UI::Color::TextBrighter;
      
      auto gizmoButton = [&c_SelectedGizmoButtonColor, buttonSize, this](const Ref<Texture>& icon,
                                                                         const ImColor& tint, float paddingY = 0.0f)
      {
        const float height = std::min((float)icon->GetHeight(), buttonSize) - paddingY * 2.0f;
        const float width = (float)icon->GetWidth() / (float)icon->GetHeight() * height;
        const bool clicked = UI::InvisibleButton(ImVec2(width, height));
        UI::DrawButtonImage(icon, tint, tint, tint, UI::RectOffset(UI::GetItemRect(), 0.0f, paddingY));
        m_hoveredGuizmoToolbar = ImGui::IsItemHovered();
        return clicked;
      };
      
      ImColor buttonTint = m_gizmoType == -1 ? c_SelectedGizmoButtonColor : c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_selectToolTex, buttonTint, m_gizmoType != -1))
      {
        m_gizmoType = -1;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::TRANSLATE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_moveToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::ROTATE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_rotateToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::ROTATE;
      }
      
      buttonTint = m_gizmoType == ImGuizmo::OPERATION::SCALE ?
      c_SelectedGizmoButtonColor :
      c_UnselectedGizmoButtonColor;
      if (gizmoButton(m_scaleToolTex, buttonTint))
      {
        m_gizmoType = ImGuizmo::OPERATION::SCALE;
      }
    }
    
    ImGui::Spring();
    ImGui::EndHorizontal();
    ImGui::Spring();
    ImGui::EndVertical();
    
    ImGui::End();
  }
} // namespace Kreator
