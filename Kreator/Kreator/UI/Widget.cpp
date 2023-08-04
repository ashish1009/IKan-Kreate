//
//  Widget.cpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#include "Widget.hpp"
#include "RendererLayer.hpp"

namespace Kreator_UI
{
  void Widgets::Initialize()
  {
    IK_LOG_TRACE("Kreator UI", "Initialising Widgets Textures");
    
    s_searchIcon = Image::Create(Kreator::RendererLayer::GetClientResorucePath() / "Textures/Icons/Search.png");
    s_gearIcon = Image::Create(Kreator::RendererLayer::GetClientResorucePath() / "Textures/Icons/Gear.png");
  }
  
  void Widgets::Shutdown()
  {
    s_searchIcon.reset();
    s_gearIcon.reset();
    
    IK_LOG_WARN("Kreator UI", "Shutting Down Widgets Textures");
  }

  bool Widgets::SearchWidget(char* searchString, uint32_t BuffSize, const char* hint, bool* grabFocus,
                             float roundingVal)
  {
    bool modified = false;
    bool searching = false;

    UI::PushID();

    // Suspend the Imgui layout to overlap the Text box with Icon
    const bool layoutSuspended = []
    {
      ImGuiWindow* window = ImGui::GetCurrentWindow();
      if (window->DC.CurrentLayout)
      {
        ImGui::SuspendLayout();
        return true;
      }
      return false;
    }();
    
    const float areaPosX = ImGui::GetCursorPosX();
    const float framePaddingY = ImGui::GetStyle().FramePadding.y;

    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, roundingVal);
    UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(28.0f, framePaddingY));

    // Input Tex box
    if (ImGui::InputText(UI::GenerateID(), searchString, BuffSize))
    {
      modified = true;
    }
    else if (ImGui::IsItemDeactivatedAfterEdit())
    {
      modified = true;
    }
    searching = searchString[0] != 0;

    if (grabFocus and *grabFocus)
    {
      if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) and
          !ImGui::IsAnyItemActive() and
          !ImGui::IsMouseClicked(0))
      {
        ImGui::SetKeyboardFocusHere(-1);
      }
      
      if (ImGui::IsItemFocused())
      {
        *grabFocus = false;
      }
    }

    UI::DrawItemActivityOutline(roundingVal, true, Kreator_UI::Color::Accent);
    ImGui::SetItemAllowOverlap();

    UI::SameLine(areaPosX + 5.0f);
    
    // Resume the Layout to overlap
    if (layoutSuspended)
    {
      ImGui::ResumeLayout();
    }

    ImGui::BeginHorizontal(UI::GenerateID(), ImGui::GetItemRectSize());
    const ImVec2 iconSize(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());
    
    // Search icon
    {
      const float iconYOffset = framePaddingY - 3.0f;
      UI::ShiftCursorY(iconYOffset);
      UI::Image(s_searchIcon, iconSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
      UI::ShiftCursorY(-iconYOffset);
      
      // Hint
      if (!searching)
      {
        UI::ShiftCursorX(10.0f);
        UI::ShiftCursorY(-framePaddingY + 1.0f);
        UI::ScopedColor text(ImGuiCol_Text, Kreator_UI::Color::TextDarker);
        UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, framePaddingY));
        ImGui::TextUnformatted(hint);
        UI::ShiftCursorY(-1.0f);
      }
    }
    ImGui::EndHorizontal();

    // Clear the Text on Escape Button
    if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Escape))
    {
      memset(searchString, 0, BuffSize);
      modified = true;
    }
    
    UI::PopID();
    return modified;
  }
} // namespace Kreator_UI
