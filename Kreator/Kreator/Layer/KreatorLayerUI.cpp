//
//  KreatorLayerUI.cpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
  void KreatorLayer::UI_WelcomePopup()
  {
    if (m_showWelcomePopup)
    {
      ImGui::OpenPopup("Welcome Screen");
      m_showWelcomePopup = false;
    }

    static constexpr bool popupFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;
    if (ImGui::BeginPopupModal("Welcome Screen", nullptr, popupFlag))
    {
      ImGui::EndPopup();
    }
  }
} // namespace Kreator
