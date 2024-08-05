//
//  ImGuiProperty.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ImGuiProperty.hpp"

#include "ImGuiInputs.hpp"

namespace Kreator::UI
{
  char* s_multilineBuffer = nullptr;
  static bool s_ReturnAfterDeactivation = false;
  
  using namespace UI;
  
  bool PropertyGridHeader(const std::string& name, bool openByDefault, float height, float rounding)
  {
    ImGuiTreeNodeFlags treeNodeFlags =  ImGuiTreeNodeFlags_Framed
    | ImGuiTreeNodeFlags_SpanAvailWidth
    | ImGuiTreeNodeFlags_AllowItemOverlap
    | ImGuiTreeNodeFlags_FramePadding;
    
    if (openByDefault)
    {
      treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    bool open = false;
    
    UI::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, rounding);
    UI::ScopedStyle headerPaddingAndHeight(ImGuiStyleVar_FramePadding, ImVec2{ height, height });
    
    ImGui::PushID(name.c_str());
    open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, Utils::String::ToUpper(name).c_str());
    ImGui::PopID();
    
    return open;
  }
  void PropertyGridHeaderEnd()
  {
    ImGui::TreePop();
  }
  
  void BeginPropertyGrid(uint32_t columns, float height, float spacing)
  {
    UI::PushID();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, spacing));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, height));
    ImGui::Columns((int32_t)columns);
  }
  void EndPropertyGrid()
  {
    ImGui::Columns(1);
    UI::DrawUnderline();
    ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
    UI::ShiftCursorY(18.0f);
    UI::PopID();
  }
  
  void HelpMarker(const char* desc)
  {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
      ImGui::BeginTooltip();
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }
  
  
  // Proeprty --------------------------------------------------------------------------------------------------------
  bool Property(const char* label, std::string& value)
  {
    bool modified = false;
    
    UI::ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    UI::ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    char buffer[256];
    strcpy(buffer, value.c_str());
    
    if (ImGui::InputText(UI::GenerateID(), buffer, 256))
    {
      value = buffer;
      modified = true;
    }
    
    if (!UI::IsItemDisabled())
    {
      UI::DrawItemActivityOutline(2.0f, true, UI::Color::Accent);
    }
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    UI::DrawUnderline();
    
    return modified;
  }
  bool PropertyMultiline(const char* label, std::string& value)
  {
    bool modified = false;
    
    ImGui::Text(label);
    ImGui::NextColumn();
    ImGui::PushItemWidth(-1);
    
    if (!s_multilineBuffer)
    {
      s_multilineBuffer = new char[1024 * 1024]; // 1KB
      memset(s_multilineBuffer, 0, 1024 * 1024);
    }
    
    strcpy(s_multilineBuffer, value.c_str());
    
    if (ImGui::InputTextMultiline(UI::GenerateID(), s_multilineBuffer, 1024 * 1024))
    {
      value = s_multilineBuffer;
      modified = true;
    }
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    
    return modified;
  }
  void Property(const char* label, const std::string& value)
  {
    UI::ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    UI::ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    UI::BeginDisabled();
    ImGui::InputText(UI::GenerateID(), (char*)value.c_str(), value.size(), ImGuiInputTextFlags_ReadOnly);
    UI::EndDisabled();
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    UI::DrawUnderline();
  }
  void Property(const char* label, const char* value)
  {
    UI::ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    UI::ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    UI::BeginDisabled();
    ImGui::InputText(UI::GenerateID(), (char*)value, 256, ImGuiInputTextFlags_ReadOnly);
    UI::EndDisabled();
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    UI::DrawUnderline();
  }
  
  bool Property(const char* label, char* value, size_t length)
  {
    UI::ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    UI::ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::InputText(UI::GenerateID(), value, length);
    
    if (!UI::IsItemDisabled())
    {
      UI::DrawItemActivityOutline(2.0f, true, Color::Accent);
    }
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    UI::DrawUnderline();
    
    return modified;
  }
  bool Property(const char* label, bool& value, const char* helpText)
  {
    bool modified = false;
    
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    if (std::strlen(helpText) != 0)
    {
      ImGui::SameLine();
      HelpMarker(helpText);
    }
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    modified = ImGui::Checkbox(GenerateID(), &value);
    
    if (!IsItemDisabled())
    {
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    }
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  bool Property(const char* label, int8_t& value, int8_t min, int8_t max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragInt8(GenerateID(), &value, 1.0f, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, int16_t& value, int16_t min, int16_t max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragInt16(GenerateID(), &value, 1.0f, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, int32_t& value, int32_t min, int32_t max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragInt32(GenerateID(), &value, 1.0f, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, int64_t& value, int64_t min, int64_t max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragInt64(GenerateID(), &value, 1.0f, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, uint8_t& value, uint8_t minValue, uint8_t maxValue)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragUInt8(GenerateID(), &value, 1.0f, minValue, maxValue);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, uint16_t& value, uint16_t minValue, uint16_t maxValue)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragUInt16(GenerateID(), &value, 1.0f, minValue, maxValue);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, uint32_t& value, uint32_t minValue, uint32_t maxValue)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragUInt32(GenerateID(), &value, 1.0f, minValue, maxValue);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, uint64_t& value, uint64_t minValue, uint64_t maxValue)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragUInt64(GenerateID(), &value, 1.0f, minValue, maxValue);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, float& value, float delta, float min, float max, const char* helpText)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    if (std::strlen(helpText) != 0)
    {
      ImGui::SameLine();
      HelpMarker(helpText);
    }
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = DragFloat(GenerateID(), &value, delta, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, glm::vec2& value, float delta, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::DragFloat2(GenerateID(), glm::value_ptr(value), delta, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, glm::vec3& value, float delta, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::DragFloat3(GenerateID(), glm::value_ptr(value), delta, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool Property(const char* label, glm::vec4& value, float delta, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::DragFloat4(GenerateID(), glm::value_ptr(value), delta, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  bool PropertySlider(const char* label, int& value, int min, int max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::SliderInt(GenerateID(), &value, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertySlider(const char* label, float& value, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::SliderFloat(GenerateID(), &value, min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertySlider(const char* label, glm::vec2& value, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::SliderFloat2(GenerateID(), glm::value_ptr(value), min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertySlider(const char* label, glm::vec3& value, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::SliderFloat3(GenerateID(), glm::value_ptr(value), min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertySlider(const char* label, glm::vec4& value, float min, float max)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::SliderFloat4(GenerateID(), glm::value_ptr(value), min, max);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, int8_t& value, int8_t step, int8_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputInt8(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, int16_t& value, int16_t step, int16_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputInt16(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, int32_t& value, int32_t step, int32_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputInt32(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, int64_t& value, int64_t step, int64_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputInt64(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, uint8_t& value, uint8_t step, uint8_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputUInt8(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, uint16_t& value, uint16_t step, uint16_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputUInt16(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, uint32_t& value, uint32_t step, uint32_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputUInt32(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyInput(const char* label, uint64_t& value, uint64_t step, uint64_t stepFast)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImGuiInputTextFlags flags = s_ReturnAfterDeactivation ? ImGuiInputTextFlags_EnterReturnsTrue : 0;
    bool modified = InputUInt64(GenerateID(), &value, step, stepFast, flags);
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyColor(const char* label, glm::vec3& value)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::ColorEdit3(GenerateID(), glm::value_ptr(value));
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  bool PropertyColor(const char* label, glm::vec4& value)
  {
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = ImGui::ColorEdit4(GenerateID(), glm::value_ptr(value));
    
    if (!IsItemDisabled())
      DrawItemActivityOutline(2.0f, true, Color::Accent);
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    DrawUnderline();
    
    return modified;
  }
  
  
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
    const char* current = options[(size_t)*selected].c_str();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool changed = false;
    
    const std::string id = "##" + std::string(strID);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (size_t i = 0; i < (size_t)optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i].c_str(), is_selected))
        {
          current = options[i].c_str();
          *selected = (int32_t)i;
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
    const char* current = options[(size_t)*selected].c_str();
    
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    bool modified = false;
    
    const std::string id = "##" + std::string(label);
    if (ImGui::BeginCombo(id.c_str(), current))
    {
      for (size_t i = 0; i < (size_t)optionCount; i++)
      {
        const bool is_selected = (current == options[i]);
        if (ImGui::Selectable(options[i].c_str(), is_selected))
        {
          current = options[i].c_str();
          *selected = (int32_t)i;
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
  bool PropertyEntityReference(const char* label, Entity& entity, const PropertyAssetReferenceSettings& settings)
  {
    bool receivedValidEntity = false;
    
    ShiftCursor(0.0f, 9.0f);
    ImGui::Text(label);
    ImGui::NextColumn();
    ShiftCursorY(4.0f);
    ImGui::PushItemWidth(-1);
    
    ImVec2 originalButtonTextAlign = ImGui::GetStyle().ButtonTextAlign;
    {
      ImGui::GetStyle().ButtonTextAlign = { 0.0f, 0.5f };
      float width = ImGui::GetContentRegionAvail().x - settings.widthOffset;
      float itemHeight = 28.0f;
      
      std::string buttonText = "Null";
      
      if (entity)
      {
        buttonText = entity.GetComponent<TagComponent>().tag;
      }
      
      ImGui::Button(GenerateLabelID(buttonText), {width, itemHeight});
    }
    ImGui::GetStyle().ButtonTextAlign = originalButtonTextAlign;
    
    if (!IsItemDisabled())
    {
      if (ImGui::BeginDragDropTarget())
      {
        auto data = ImGui::AcceptDragDropPayload("scene_entity_hierarchy");
        if (data)
        {
          entity = *(Entity*)data->Data;
          receivedValidEntity = true;
        }
        
        ImGui::EndDragDropTarget();
      }
    }
    
    ImGui::PopItemWidth();
    
    if (settings.advanceToNextColumn)
    {
      ImGui::NextColumn();
      UI::ShiftCursorY(4.0f);
      ImGui::PushItemWidth(-1);
    }
    
    return receivedValidEntity;
  }
} // namespace Kreator::UI
