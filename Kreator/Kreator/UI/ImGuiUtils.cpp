//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  // Image / Texture --------------------------------------------------------------------------------------------------
  void Image(const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec4& tintCol, const ImVec4& borderCol, const ImVec2& uv0, const ImVec2& uv1)
  {
    ImGui::Image(INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, tintCol, borderCol);
  }
} //namespace Kreator::UI
