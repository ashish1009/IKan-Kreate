//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  static int32_t s_UIContextID = 0;
  static uint32_t s_counter = 0;
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
    std::string result = "##";
    result += std::to_string(s_counter++);
    memcpy(s_bufferID, result.c_str(), 16);
    return s_bufferID;
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
} // namespace Kreator::UI
