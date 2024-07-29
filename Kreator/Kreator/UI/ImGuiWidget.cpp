//
//  ImGuiWidget.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ImGuiWidget.hpp"

#include <text/choc_StringUtilities.h>

#include "Layers/RendererLayer.hpp"

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
    
    s_searchIcon = TextureFactory::Create(Kreator::RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Search.png");
    s_gearIcon = TextureFactory::Create(Kreator::RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
    
    s_assetSearchString = new char[MAX_ASSET_INPUT];
  }
  
  void Widgets::Shutdown()
  {
    IK_PROFILE();
    s_searchIcon.reset();
    s_gearIcon.reset();
    delete[] s_assetSearchString;
    
    IK_LOG_INFO("Kreator UI", "Shutting Down Widgets Textures");
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
    UI::DrawButtonImage(s_gearIcon, UI::RectExpanded(UI::GetItemRect(), -padding, -padding),
                        IM_COL32(value, value, value, 200),
                        IM_COL32(value, value, value, 255),
                        IM_COL32(value, value, value, 150));
    return clicked;
  }
  
  bool Widgets::AssetSearchPopup(const char* ID, AssetType assetType, AssetHandle& selected, bool allowMemoryOnlyAssets,
                                 bool* cleared, const char* hint /*= "Search Assets"*/, const ImVec2& size)
  {
    bool modified = false;
    UI::ScopedColor popupBG(ImGuiCol_PopupBg, UI::ColorWithMultipliedValue(UI::Color::Background, 1.6f));
    
    AssetHandle current = selected;
    ImGui::SetNextWindowSize({ size.x, 0.0f });
    static bool grabFocus = true;
    if (UI::BeginPopup(ID, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
    {
      if (ImGui::GetCurrentWindow()->Appearing)
      {
        grabFocus = true;
        memset(s_assetSearchString, 0, MAX_ASSET_INPUT);
      }
      
      // Search widget
      UI::ShiftCursor(0.0f, 2.0f);
      ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() * 2.0f);
      SearchWidget(s_assetSearchString, MAX_ASSET_INPUT, hint, &grabFocus);
      
      bool searching = s_assetSearchString[0] != 0;
      // Clear property button
      if (cleared != nullptr)
      {
        UI::ScopedColorStack buttonColours(
                                           ImGuiCol_Button, UI::ColorWithMultipliedValue(UI::Color::Muted, 1.0f),
                                           ImGuiCol_ButtonHovered, UI::ColorWithMultipliedValue(UI::Color::Muted, 1.2f),
                                           ImGuiCol_ButtonActive, UI::ColorWithMultipliedValue(UI::Color::Muted, 0.9f));
        
        UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);
        UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 10);
        
        ImGui::SetCursorPosX(0);
        ImGui::PushItemFlag(ImGuiItemFlags_NoNav, searching);
        
        UI::ShiftCursorX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize("CLEAR").x / 2);
        if (ImGui::Button("CLEAR"))
        {
          *cleared = true;
          modified = true;
        }
        
        ImGui::PopItemFlag();
      }
      
      
      // List of assets
      {
        UI::ScopedColor listBoxBg(ImGuiCol_FrameBg, IM_COL32_DISABLE);
        UI::ScopedColor listBoxBorder(ImGuiCol_Border, IM_COL32_DISABLE);
        
        ImGuiID listID = ImGui::GetID("##SearchListBox");
        if (ImGui::BeginListBox("##SearchListBox", ImVec2(-FLT_MIN, 0.0f)))
        {
          bool forwardFocus = false;
          
          ImGuiContext& g = *GImGui;
          if (g.NavJustMovedToId != 0)
          {
            if (g.NavJustMovedToId == listID)
            {
              forwardFocus = true;
              
              // ActivateItem moves keyboard navigation focuse inside of the window
              ImGui::ActivateItem(listID);
              ImGui::SetKeyboardFocusHere(1);
            }
          }
          
          const auto& assetRegistry = AssetManager::GetAssetRegistry();
          std::vector<std::string> visitedAsset;
          for (auto it = assetRegistry.cbegin(); it != assetRegistry.cend(); it++)
          {
            const auto& [path, metadata] = *it;
            
            if (allowMemoryOnlyAssets != metadata.isMemoryAsset)
            {
              continue;
            }
            
            if (metadata.type != assetType)
            {
              continue;
            }
            
            const std::string assetName = metadata.isMemoryAsset ? metadata.filePath.string() : metadata.filePath.stem().string();
            if (std::find(visitedAsset.begin(), visitedAsset.end(), assetName) != visitedAsset.end())
            {
              continue;
            }
            visitedAsset.emplace_back(assetName);
            
            if (s_assetSearchString[0] != 0 and !UI::IsMatchingSearch(assetName, s_assetSearchString))
            {
              continue;
            }
            
            bool isSelected = (current == metadata.handle);
            if (ImGui::Selectable(assetName.c_str(), isSelected))
            {
              current = metadata.handle;
              selected = metadata.handle;
              modified = true;
            }
            
            if (forwardFocus)
            {
              forwardFocus = false;
            }
            else if (isSelected)
            {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndListBox();
        }
      }
      
      if (modified)
      {
        ImGui::CloseCurrentPopup();
      }
      
      UI::EndPopup();
    }
    return modified;
  }
} // namespace Kreator::UI
