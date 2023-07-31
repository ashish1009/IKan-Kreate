//
//  Theme.hpp
//  Kreator
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace Kreator_UI
{
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

} // namespace Kreator_UI
