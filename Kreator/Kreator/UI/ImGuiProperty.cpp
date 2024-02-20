//
//  ImGuiProperty.cpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
//

#include "ImGuiProperty.hpp"

namespace Kreator::UI
{
  bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected)
  {
    const char* current = options[*selected];
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = false;
    
    const std::string id = "##" + std::string(label);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (int i = 0; i < optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i], is_selected))
        {
          current = options[i];
          *selected = i;
          modified = true;
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyDropdownNoLabel(const char* strID, const char** options, int32_t optionCount, int32_t* selected)
  {
    const char* current = options[*selected];
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool changed = false;
    
    const std::string id = "##" + std::string(strID);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (int i = 0; i < optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i], is_selected))
        {
          current = options[i];
          *selected = i;
          changed = true;
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return changed;
  }
  
  bool PropertyDropdownNoLabel(const char* strID, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected)
  {
    const char* current = options[*selected].c_str();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool changed = false;
    
    const std::string id = "##" + std::string(strID);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (int i = 0; i < optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i].c_str(), is_selected))
        {
          current = options[i].c_str();
          *selected = i;
          changed = true;
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return changed;
  }
  
  bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected)
  {
    const char* current = options[*selected].c_str();
    
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = false;
    
    const std::string id = "##" + std::string(label);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (int i = 0; i < optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i].c_str(), is_selected))
        {
          current = options[i].c_str();
          *selected = i;
          modified = true;
        }
        if (is_selected)
          ImGui::SetItemDefaultFocus();
      }
      ImGui::EndCombo();
    }
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
} // Kreator::UI
