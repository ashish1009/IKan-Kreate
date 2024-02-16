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

    // Keep the window at center
    UI::SetNextWindowAtCenterWithSize({1000, 500});

    static constexpr bool popupFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;
    if (ImGui::BeginPopupModal("Welcome Screen", nullptr, popupFlag))
    {
      UI::PushID();

      ImGuiTableFlags tableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        
        ImGui::EndTable();
      } // Begin Table
      UI::PopID();
      ImGui::EndPopup();
    } // Begin Popup Model
  }
} // namespace Kreator
