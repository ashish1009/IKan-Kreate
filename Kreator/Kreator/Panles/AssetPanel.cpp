//
//  AssetPanel.cpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#include "AssetPanel.hpp"

namespace Kreator
{
  void AssetPanel::OnImGuiRender(bool& isOpen)
  {
    if (!isOpen)
    {
      return;
    }
    
    ImGui::Begin("Asset Manager", &isOpen);

    ImGui::End();
  }
} // namespace Kreator
