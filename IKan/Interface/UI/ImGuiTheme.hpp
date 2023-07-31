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
      inline static ImU32 TextBrighter         = IM_COL32(200, 200, 200, 255);
    };
    
    /// This function changes the imgui font
    ///  - Parameter otherFonts: other fonts
    static void ChangeFont(const std::vector<UI::Font> otherFonts);
  };
} // namespace IKan::UI
