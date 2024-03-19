//
//  ImGuiTheme.hpp
//  Kreator
//
//  Created by Ashish . on 19/03/24.
//

#pragma once

#include "Editor/UserPreferences.hpp"

namespace Kreator::UI
{
  struct Color
  {
  };
  /// This function changes the imgui font
  ///  - Parameter otherFonts: other fonts
  void LoadFonts(const std::vector<IKan::UI::ImGuiFont>& otherFonts);
} // namespace Kreator::UI
