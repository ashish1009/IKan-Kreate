//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  static int32_t s_UIContextID{0};
  static uint32_t s_counter{0};
  static char s_bufferID[16] = "##";

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
    std::string result {"##"};
    result += std::to_string(s_counter++);
    memcpy(s_bufferID, result.c_str(), 16);
    return s_bufferID;
  }
  ImTextureID GetTextureID(Ref<IKan::Texture> texture)
  {
    return (ImTextureID)INT2VOIDP(texture->GetRendererID());
  }

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
  
  void BeginDisabled(bool disabled)
  {
    if (disabled)
    {
      ImGui::BeginDisabled(true);
    }
  }
  void EndDisabled()
  {
    if (GImGui->DisabledStackSize > 0)
    {
      ImGui::EndDisabled();
    }
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
  void SetCursorPos(const ImVec2& val)
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

} // namespace Kreator::UI
