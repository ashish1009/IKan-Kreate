//
//  ImGuiScoped.hpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

namespace Kreator::UI
{
  /// This class creates scoped style variable for ImGui
  class ScopedStyle
  {
  public:
    template <typename T> ScopedStyle(ImGuiStyleVar styleVar, T value)
    {
      ImGui::PushStyleVar(styleVar, value);
    }
    ~ScopedStyle()
    {
      ImGui::PopStyleVar();
    }
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedStyle);
  };
} // namespace Kreator::UI
