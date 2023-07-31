//
//  ImGuiTheme.hpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

#include "UI/ImGuiLayer.hpp"

namespace IKan::UI
{
  struct Theme
  {
    // Default Theme Color -------------------------------------------------------------------------------------------
    struct Color
    {
      inline static ImU32 Text                 = IM_COL32(255, 255, 255, 255);
      inline static ImU32 Titlebar             = IM_COL32(18, 18, 18, 255);
      inline static ImU32 Background           = IM_COL32(0, 0, 0, 255);
      inline static ImU32 BackgroundPopup      = IM_COL32(22, 22, 22, 255);
      inline static ImU32 BackgroundDark       = IM_COL32(11, 11, 11, 255);
      inline static ImU32 PropertyField        = IM_COL32(15, 15, 15, 255);
      inline static ImU32 GroupHeader          = IM_COL32(37, 37, 37, 255);
      inline static ImU32 GroupHeaderHovered   = IM_COL32(32, 32, 32, 255);
      inline static ImU32 GroupHeaderActive    = IM_COL32(30, 30, 30, 255);
      
      inline static ImU32 Highlight            = IM_COL32(12, 12, 242, 255);
      inline static ImU32 Accent               = IM_COL32(0, 0, 255, 255);
      inline static ImU32 NiceBlue             = IM_COL32(10, 10, 254, 255);
      inline static ImU32 Compliment           = IM_COL32(9, 9, 9, 255);
      inline static ImU32 TextBrighter         = IM_COL32(210, 210, 210, 255);
      inline static ImU32 TextDarker           = IM_COL32(128, 128, 128, 255);
      inline static ImU32 TextError            = IM_COL32(230, 51, 51, 255);
      inline static ImU32 Muted                = IM_COL32(47, 47, 47, 255);
      inline static ImU32 Selection            = IM_COL32(191, 191, 191, 255);
      inline static ImU32 SelectionMuted       = IM_COL32(57, 57, 57, 255);
      inline static ImU32 HoveredItem          = IM_COL32(0, 0, 0, 80);
    };
    
    /// This function changes the imgui font
    ///  - Parameters:
    ///   - defaultFont: Default font informatiom
    ///   - boldFont: bold Font informatiom
    ///   - otherFonts: other fonts
    static void ChangeFont(const UI::Font &defaultFont, const UI::Font &boldFont, const std::vector<UI::Font> otherFonts = {});
    
    /// This function Returns the regular font
    static ImFont* GetRegularFont();
    /// This function Returns the bold font
    static ImFont* GetBoldFont();
  };
} // namespace IKan::UI
