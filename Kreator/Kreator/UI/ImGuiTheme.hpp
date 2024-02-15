//
//  ImGuiTheme.hpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#pragma once

#include "Editor/UserPreferences.hpp"

namespace Kreator::UI
{
  struct Color
  {
    inline static ImU32 Text                 = IM_COL32(222, 222, 222, 255);
    inline static ImU32 TextBrighter         = IM_COL32(210, 210, 210, 255);
    inline static ImU32 TextDarker           = IM_COL32(128, 128, 128, 255);
    inline static ImU32 TextError            = IM_COL32(230, 51, 51, 255);
    inline static ImU32 Background           = IM_COL32(28, 28, 33, 255);
    inline static ImU32 Titlebar             = IM_COL32(35, 45, 60, 255);
    inline static ImU32 TitleBarDark         = IM_COL32(25, 25, 40, 255);
    inline static ImU32 ActiveTab            = IM_COL32(71, 75, 81, 255);
    inline static ImU32 BackgroundPopup      = IM_COL32(43, 63, 91, 255);
    inline static ImU32 BackgroundDark       = IM_COL32(21, 21, 21, 255);
    inline static ImU32 WindowDimBg          = IM_COL32(15, 15, 15, 255);
    inline static ImU32 PropertyField        = IM_COL32(25, 35, 45, 255);
    inline static ImU32 GroupHeader          = IM_COL32(27, 27, 27, 255);
    inline static ImU32 GroupHeaderHovered   = IM_COL32(32, 32, 32, 255);
    inline static ImU32 GroupHeaderActive    = IM_COL32(30, 30, 30, 255);
    inline static ImU32 Muted                = IM_COL32(77, 77, 77, 255);
    
    inline static ImU32 Warning              = IM_COL32(183, 32, 54, 255);
    inline static ImU32 NiceThemeHighlight   = IM_COL32(83, 232, 254, 255);
    inline static ImU32 Highlight            = IM_COL32(39, 185, 242, 255);
    inline static ImU32 Accent               = IM_COL32(236, 158, 36, 255);
    inline static ImU32 Selection            = IM_COL32(191, 177, 155, 255);
    inline static ImU32 SelectionMuted       = IM_COL32(59, 57, 45, 255);
    inline static ImU32 HoveredItem          = IM_COL32(0, 0, 0, 80);
  };
  
  /// This function changes theme color to dark
  void SetThemeColors(Kreator::UserPreferences::Theme theme);
  /// This function changes the imgui font
  ///  - Parameter otherFonts: other fonts
  void LoadFonts(const std::vector<IKan::UI::ImGuiFont>& otherFonts);

  /// This function Returns the regular font
  ImFont* GetRegularFont();
  /// This function Returns the bold font
  ImFont* GetBoldFont();
  /// This function Returns the italic font
  ImFont* GetItalicFont();
  /// This function Returns the Fixed width font
  ImFont* GetFixedWidthFont();
  /// This function Returns the Huge Header font
  ImFont* GetHugeHeaderFont();
  /// This function Returns the Semi Header font
  ImFont* GetSemiHeaderFont();

} // namespace Kreator::UI
