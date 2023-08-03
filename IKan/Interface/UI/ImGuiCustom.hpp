//
//  ImGuiCustom.hpp
//  IKan
//
//  Created by Ashish . on 03/08/23.
//

#pragma once

namespace IKan
{
  class Image;
} // namespace IKan

namespace IKan::UI
{
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end,
                        ImColor iconTint = IM_COL32_WHITE);
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, const void* ptr_id, ImGuiTreeNodeFlags flags, ImColor iconTint, const char* fmt, ...);
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, const char* label, ImGuiTreeNodeFlags flags, ImColor iconTint = IM_COL32_WHITE);
} // namespace IKan::UI
