//
//  ImGuiScoped.hpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

#include "UI/ImGuiUtils.hpp"

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
  
  /// This class creates scoped color in stack for ImGui
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
  
  /// This class creates scoped Font for ImGui
  class ScopedFont
  {
  public:
    ScopedFont(ImFont* font)
    {
      ImGui::PushFont(font);
    }
    ~ScopedFont()
    {
      ImGui::PopFont();
    }
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedFont);
  };
  
  /// This class creates scoped ID for ImGui
  class ScopedID
  {
  public:
    ScopedID(const ScopedID&) = delete;
    ScopedID& operator=(const ScopedID&) = delete;
    template<typename T> ScopedID(T id)
    {
      ImGui::PushID(id);
    }
    ~ScopedID()
    {
      ImGui::PopID();
    }
  };
  
  /// This class creates scoped table for ImGui
  class ScopedTable
  {
  public:
    /// This structure stores the table column data
    struct TableColumns
    {
      std::string_view tag;
      float width{-1};
    };
    
    ScopedTable(const ScopedID&) = delete;
    ScopedTable& operator=(const ScopedID&) = delete;
    
    ScopedTable(const std::initializer_list<TableColumns>& columns, ImGuiTableFlags tableFlags = 0)
    : m_columns(columns)
    {
      IK_ASSERT(m_columns.size());
      
      ImGui::BeginTable(UI::GenerateID(), static_cast<int32_t>(m_columns.size()), tableFlags | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV, ImVec2(0.0f, 0.0f));
      
      for (const TableColumns& column : m_columns)
      {
        ImGuiTableColumnFlags flag = 0;
        if (column.width == -1)
        {
          flag = ImGuiTableColumnFlags_WidthStretch;
        }
        ImGui::TableSetupColumn(column.tag.data(), flag, column.width);
      }
      ImGui::TableNextRow();
    }
    ~ScopedTable()
    {
      ImGui::EndTable();
    }
    
    /// This function shows the popup in imgui loop
    /// - Parameters:
    ///   - popupFlags: popup flags
    ///   - func: function inside popup
    template<typename FuncT> void ShowColumn(size_t index, FuncT&& func)
    {
      IK_ASSERT(index < m_columns.size())
      
      ImGui::TableSetColumnIndex((int32_t)index);
      ImGui::BeginChild(m_columns.at(index).tag.data());
      {
        func();
        ImGui::EndChild();
      }
    }
    
  private:
    std::vector<TableColumns> m_columns;
  };
} // namespace Kreator::UI
