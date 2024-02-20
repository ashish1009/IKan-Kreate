//
//  ImGuiWidget.cpp
//  Kreator
//
//  Created by Ashish . on 19/02/24.
//

#include "ImGuiWidget.hpp"
#include <text/choc_StringUtilities.h>
#include "Layer/KreatorLayer.hpp"

namespace Kreator::UI
{
#define MAX_ASSET_INPUT 128
  
  bool IsMatchingSearch(const std::string& item, const std::string_view& searchQuery, bool caseSensitive, bool stripWhiteSpaces, bool stripUnderscores)
  {
    if (searchQuery.empty())
    {
      return true;
    }
    
    if (item.empty())
    {
      return false;
    }
    
    std::string itemSanitized = stripUnderscores ? choc::text::replace(item, "_", " ") : item;
    
    if (stripWhiteSpaces)
    {
      itemSanitized = choc::text::replace(itemSanitized, " ", "");
    }
    
    std::string searchString = stripWhiteSpaces ? choc::text::replace(searchQuery, " ", "") : std::string(searchQuery);
    
    if (!caseSensitive)
    {
      itemSanitized = Utils::String::ToLower(itemSanitized);
      searchString = Utils::String::ToLower(searchString);
    }
    
    bool result = false;
    if (choc::text::contains(searchString, " "))
    {
      std::vector<std::string> searchTerms = choc::text::splitAtWhitespace(searchString);
      for (const auto& searchTerm : searchTerms)
      {
        if (!searchTerm.empty() and choc::text::contains(itemSanitized, searchTerm))
        {
          result = true;
        }
        else
        {
          result = false;
          break;
        }
      }
    }
    else
    {
      result = choc::text::contains(itemSanitized, searchString);
    }
    
    return result;
  }
  
  void Widgets::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator UI", "Initialising Widgets Textures");
    
    s_searchIcon = TextureFactory::Create(Kreator::KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Search.png");
    s_gearIcon = TextureFactory::Create(Kreator::KreatorLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
    
    s_assetSearchString = new char[MAX_ASSET_INPUT];
  }
  
  void Widgets::Shutdown()
  {
    IK_PROFILE();
    s_searchIcon.reset();
    s_gearIcon.reset();
    delete[] s_assetSearchString;
    
    IK_LOG_WARN("Kreator UI", "Shutting Down Widgets Textures");
  }
  
  bool Widgets::SearchWidget(char* searchString, uint32_t BuffSize, const char* hint, bool* grabFocus, float roundingVal)
  {
    bool modified = false;
    bool searching = false;
    
    // Suspend the ImGui layout to overlap the Text box with Icon
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
    
    UI::PushID();
    {
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
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) and !ImGui::IsAnyItemActive() and !ImGui::IsMouseClicked(0))
        {
          ImGui::SetKeyboardFocusHere(-1);
        }
        if (ImGui::IsItemFocused())
        {
          *grabFocus = false;
        }
      }
      
      UI::DrawItemActivityOutline(roundingVal, true, UI::Color::Accent);
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
        UI::Image(s_searchIcon, iconSize);
        UI::ShiftCursorY(-iconYOffset);
        
        // Hint
        if (!searching)
        {
          UI::ShiftCursorX(10.0f);
          UI::ShiftCursorY(-framePaddingY + 1.0f);
          UI::ScopedColor text(ImGuiCol_Text, UI::Color::TextDarker);
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
    }
    UI::PopID();
    
    return modified;
  }
  
  bool Widgets::OptionsButton()
  {
    const bool clicked = ImGui::InvisibleButton("##options", ImVec2{ ImGui::GetFrameHeight(), ImGui::GetFrameHeight() });
    
    const float spaceAvail = std::min(ImGui::GetItemRectSize().x, ImGui::GetItemRectSize().y);
    const float desiredIconSize = 15.0f;
    const float padding = std::max((spaceAvail - desiredIconSize) / 2.0f, 0.0f);
    
    auto buttonColor = UI::Color::Text;
    const uint8_t value = uint8_t(ImColor(buttonColor).Value.x * 255);
    UI::DrawButtonImage(s_gearIcon, IM_COL32(value, value, value, 200), IM_COL32(value, value, value, 255),
                        IM_COL32(value, value, value, 150), UI::RectExpanded(UI::GetItemRect(), -padding, -padding));
    return clicked;
  }

} // namespace Kreator::UI
