//
//  ImGuiUtils.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiUtils.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan::UI
{
  static int32_t s_UIContextID = 0;
  static uint32_t s_counter = 0;
  static char s_bufferID[16] = "##";
  static char s_labeledBufferID[1024];

  // Wrappers --------------------------------------------------------------------------------------------------------
  void Separator()
  {
    ImGui::Separator();
  }
  ImTextureID GetTextureID(Ref<Texture> texture)
  {
    return (ImTextureID)INT2VOIDP(texture->GetRendererID());
  }
  void ShiftCursorX(float distance)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
  }
  void ShiftCursorY(float distance)
  {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
  }
  void ShiftCursor(float x, float y)
  {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
  }
  void ShiftCursor(ImVec2 val)
  {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + val.x, cursor.y + val.y));
  }
  
  void SetCursorPos(float x, float y)
  {
    ImGui::SetCursorPos({x, y});
  }
  void SetCursorPos(ImVec2 val)
  {
    ImGui::SetCursorPos(val);
  }
  void SetCursorPosX(float x)
  {
    ImGui::SetCursorPosX(x);
  }
  void SetCursorPosY(float y)
  {
    ImGui::SetCursorPosY(y);
  }
  
  void SameLine(float offsetFromStartX, float spacing)
  {
    ImGui::SameLine(offsetFromStartX, spacing);
  }
  void NewLine()
  {
    ImGui::NewLine();
  }
  
  bool IsItemHovered()
  {
    return ImGui::IsItemHovered();
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
  
  const char* GenerateID()
  {
    std::string result = "##";
    result += std::to_string(s_counter++);
    memcpy(s_bufferID, result.c_str(), 16);
    return s_bufferID;
  }
  
  const char* GenerateLabelID(std::string_view label)
  {
    *fmt::format_to_n(s_labeledBufferID, std::size(s_labeledBufferID), "{}##{}", label, s_counter++).out = 0;
    return s_labeledBufferID;
  }
  
  void SetMouseEnabled(const bool enable)
  {
    if (enable)
    {
      ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    else
    {
      ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
  }
  
  bool NavigatedTo()
  {
    ImGuiContext& g = *GImGui;
    return g.NavJustMovedToId == g.LastItemData.ID;
  }
} // namespace IKan::UI
