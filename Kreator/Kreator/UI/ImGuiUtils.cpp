//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  static int32_t s_UIContextID = 0;
  static uint32_t s_counter = 0;

  // Wrappers --------------------------------------------------------------------------------------------------------
  void SetNextWindowAtCenter()
  {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  }
  void SetNextWindowAtCenterWithSize(const ImVec2& size)
  {
    SetNextWindowAtCenter();
    SetNextWindowSize(size);
  }
  void SetNextWindowPos(const ImVec2& pos)
  {
    ImGui::SetNextWindowPos(pos);
  }
  void SetNextWindowSize(const ImVec2& size)
  {
    ImGui::SetNextWindowSize(size);
  }

  void PushID()
  {
    ImGui::PushID(s_UIContextID++);
    s_counter = 0;
  }
  void PopID()
  {
    ImGui::PopID();
    s_UIContextID--;
  }

  // Image / Texture --------------------------------------------------------------------------------------------------
  void Image(const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec4& tintCol, const ImVec4& borderCol, const ImVec2& uv0, const ImVec2& uv1)
  {
    ImGui::Image(INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, tintCol, borderCol);
  }
} //namespace Kreator::UI
