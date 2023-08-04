//
//  ImGuiProperty.cpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#include "ImGuiProperty.hpp"
#include "UI/ImGuiScoped.hpp"

namespace IKan::UI
{
  bool PropertyGridHeader(const std::string& name, bool openByDefault)
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
    const float framePaddingX = 6.0f;
    const float framePaddingY = 6.0f; // affects height of the header
    
    UI::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, 0.0f);
    UI::ScopedStyle headerPaddingAndHeight(ImGuiStyleVar_FramePadding, ImVec2{ framePaddingX, framePaddingY });
    
    ImGui::PushID(name.c_str());
    open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, Utils::String::ToUpper(name).c_str());
    ImGui::PopID();
    
    return open;
  }
  void PropertyGridHeaderEnd()
  {
    ImGui::TreePop();
  }
} // namespace IKan::UI
