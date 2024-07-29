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
  
  /// This class creates scoped color for ImGui
  class ScopedColor
  {
  public:
    template<typename T> ScopedColor(ImGuiCol colourId, T colour)
    {
      ImGui::PushStyleColor(colourId, colour);
    }
    ~ScopedColor()
    {
      ImGui::PopStyleColor();
    }
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedColor);
  };
} // namespace Kreator::UI
