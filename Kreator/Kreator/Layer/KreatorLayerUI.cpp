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
//              ImGui::CloseCurrentPopup();
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

        } // Begin child About
        ImGui::EndChild(); // About/New_Project
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
        ImGui::SetKeyboardFocusHere();
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
        ImGui::PushFont(UI::GetBoldFont());
        
        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - 90);
        if ((UI::DrawRoundButton("Create", UI::Color::NiceThemeHighlight, 10)) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
        {
          std::filesystem::path fullProjectPath {};
          if (strlen(m_projectFilePathBuffer.Data()) > 0)
          {
            fullProjectPath = std::filesystem::path(m_projectFilePathBuffer) / std::filesystem::path(m_projectNameBuffer);
          }
          
          CreateProject(fullProjectPath);
          ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        if ((UI::DrawRoundButton("Cancel", UI::Color::Muted, 10)) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
        {
//          ImGui::CloseCurrentPopup();
        }
        ImGui::PopFont();
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
      case FolderExplorerAction::None:
      default:
        IK_ASSERT(false);
        break;
    }
  }
} // namespace Kreator
