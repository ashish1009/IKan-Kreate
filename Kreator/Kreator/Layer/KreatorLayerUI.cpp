//
//  KreatorLayerUI.cpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#include "KreatorLayer.hpp"

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
              
              const ImU32 buttonColN = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 0.9f);
              const ImU32 buttonColH = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 1.2f);
              const ImU32 buttonColP = UI::ColorWithMultipliedValue(IM_COL32(222, 222, 222, 255), 1.0f);
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
            }
            
            if (button("Open Project", m_folderIcon, "Open an exisiting Kreator Project"))
            {
              ImGui::CloseCurrentPopup();
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
} // namespace Kreator
