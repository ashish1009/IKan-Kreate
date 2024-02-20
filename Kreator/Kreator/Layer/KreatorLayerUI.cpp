//
//  KreatorLayerUI.cpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#include "KreatorLayer.hpp"
#include "Editor/FolderExplorer.hpp"

namespace Kreator
{
  void KreatorLayer::UI_WelcomePopup()
  {
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
              FolderExplorer::Open(ProjectExtension, &m_showWelcomePopup);
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
      case FolderExplorerAction::None:
      default:
        IK_ASSERT(false);
        break;
    }
  }
  
  void KreatorLayer::UI_StartMainWindowDocking()
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
    ImGui::End();
  }
  
  float KreatorLayer::UI_DrawTitlebar()
  {
    static const float titleBarHeight = 40.0f;
    
    // Draw the title Bar rectangle ---------------------------------------------------
    static const ImVec2 titlebarMin = ImGui::GetCursorScreenPos();
    static const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth(),
      ImGui::GetCursorScreenPos().y + titleBarHeight
    };
    
    static auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, UI::Color::Titlebar);
    
    // Drag and Control the window with user title bar ---------------------------------
    UI_TitlebarDragArea(titleBarHeight);

    // Draw Kreator Logo ---------------------------------------------------------------
    static const int32_t logoWidth = titleBarHeight - 10;
    static const int32_t logoHeight = titleBarHeight - 10;
    static const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x,
      ImGui::GetItemRectMin().y
    };
    static const ImVec2 logoRectMax =
    {
      logoRectStart.x + logoWidth,
      logoRectStart.y + logoHeight
    };
    drawList->AddImage(UI::GetTextureID(m_applicationIcon), logoRectStart, logoRectMax, {0, 1}, {1, 0});

    
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
} // namespace Kreator
