//
//  ImGuiScoped.hpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace IKan::UI
{
  /// Scoped ImGui Stype Var
  /// - Note: Should be inside Scope
  class ScopedStyle
  {
  public:
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedStyle);
    template<typename T> ScopedStyle(ImGuiStyleVar style_var, T value)
    {
      ImGui::PushStyleVar(style_var, value);
    }
    ~ScopedStyle()
    {
      ImGui::PopStyleVar();
    }
  };
  
  /// Scoped ImGui Color Var
  /// - Note: Should be inside Scope
  class ScopedColor
  {
  public:
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedColor);
    template<typename T> ScopedColor(ImGuiCol colourId, T colour)
    {
      ImGui::PushStyleColor(colourId, colour);
    }
    ~ScopedColor()
    {
      ImGui::PopStyleColor();
    }
  };
  
  class ScopedFont
  {
  public:
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedFont);
    ScopedFont(ImFont* font)
    {
      ImGui::PushFont(font);
    }
    ~ScopedFont()
    {
      ImGui::PopFont();
    }
  };
  
  class ScopedColorStack
  {
  public:
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedColorStack);
    template <typename ColorType, typename... OtherColors>
    ScopedColorStack(ImGuiCol first_color_id, ColorType first_color, OtherColors&& ... otherColorPairs)
    : m_count((sizeof... (otherColorPairs) / 2) + 1)
    {
      static_assert ((sizeof... (otherColorPairs) & 1u) == 0,
                     "ScopedColorStack constructor expects a list of pairs of colour IDs and colours as its arguments");
      PushColour(first_color_id, first_color, std::forward<OtherColors>(otherColorPairs)...);
    }
    ~ScopedColorStack()
    {
      ImGui::PopStyleColor(m_count);
    }
    
  private:
    int32_t m_count;
    
    template <typename ColorType, typename... OtherColors>
    void PushColour(ImGuiCol color_id, ColorType color, OtherColors&& ... other_color_pairs)
    {
      if constexpr (sizeof... (other_color_pairs) == 0)
      {
        ImGui::PushStyleColor(color_id, color);
      }
      else
      {
        ImGui::PushStyleColor(color_id, color);
        PushColour(std::forward<OtherColors>(other_color_pairs)...);
      }
    }
  };
  
  class ScopedDisable
  {
  public:
    ScopedDisable(const ScopedDisable&) = delete;
    ScopedDisable& operator=(const ScopedDisable&) = delete;
    ScopedDisable(bool disabled = true);
    ~ScopedDisable();
  };
  
  class ScopedID
  {
  public:
    ScopedID(const ScopedID&) = delete;
    ScopedID& operator=(const ScopedID&) = delete;
    template<typename T>
    ScopedID(T id) { ImGui::PushID(id); }
    ~ScopedID() { ImGui::PopID(); }
  };
  
} // namespace IKan::UI
