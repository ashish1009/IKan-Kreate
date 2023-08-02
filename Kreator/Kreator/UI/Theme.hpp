//
//  Theme.hpp
//  Kreator
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace Kreator_UI
{
  namespace Color
  {
    static ImU32 Highlight            = IM_COL32(39, 185, 242, 255);
    static ImU32 Accent               = IM_COL32(236, 158, 36, 255);
    static ImU32 NiceBlue             = IM_COL32(83, 232, 254, 255);
    static ImU32 Compliment           = IM_COL32(78, 151, 166, 255);
    static ImU32 TextDarker           = IM_COL32(128, 128, 128, 255);
    static ImU32 TextError            = IM_COL32(230, 51, 51, 255);
    static ImU32 Muted                = IM_COL32(77, 77, 77, 255);
    static ImU32 Selection            = IM_COL32(191, 177, 155, 255);
    static ImU32 SelectionMuted       = IM_COL32(59, 57, 45, 255);
    static ImU32 HoveredItem          = IM_COL32(0, 0, 0, 80);
    static ImU32 TitleBarDark         = IM_COL32(25, 25, 40, 255);
  } // namespace Color
  
  /// This function changes theme color to dark
  void SetDarkTheme();
  
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
  
  /// This function convert the Im32 Color to glm 3
  /// - Parameter color: color
  glm::vec3 ColorVec3FromU32(ImU32 color);

} // namespace Kreator_UI
