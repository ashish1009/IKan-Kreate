//
//  Theme.hpp
//  Kreator
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace Kreator_UI
{
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
