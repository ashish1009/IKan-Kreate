//
//  Theme.cpp
//  Kreator
//
//  Created by Ashish . on 31/07/23.
//

#include "Theme.hpp"

namespace Kreator_UI
{
  ImFont* GetRegularFont()
  {
    return ImGui::GetIO().Fonts->Fonts[0];
  }
  ImFont* GetBoldFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[1] : fonts[0];
  }
  ImFont* GetItalicFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[2] : fonts[0];
  }
  ImFont* GetFixedWidthFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[3] : fonts[0];
  }
  ImFont* GetHugeHeaderFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[4] : fonts[0];
  }
  ImFont* GetSemiHeaderFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[5] : fonts[0];
  }
  
  glm::vec3 ColorVec3FromU32(ImU32 color)
  {
    ImVec4 result = ImGui::ColorConvertU32ToFloat4(color);
    return {result.x, result.y, result.z};
  }
} // namespace Kreator_UI
