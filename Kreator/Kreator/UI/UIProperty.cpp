//
//  ImGuiProperty.cpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#include "UIProperty.hpp"

namespace Kreator_UI
{
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
  
  void BeginPropertyGrid(uint32_t columns, float height)
  {
    UI::PushID();
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4.0f, height));
    ImGui::Columns(columns);
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
      UI::DrawItemActivityOutline(2.0f, true, Kreator_UI::Color::Accent);
    }
    
    ImGui::PopItemWidth();
    ImGui::NextColumn();
    UI::DrawUnderline();
    
    return modified;
  }

} // namespace IKan::UI
