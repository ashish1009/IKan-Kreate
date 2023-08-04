//
//  KreatorConsolePanel.cpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#include "KreatorConsolePanel.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  KreatorConsolePanel::KreatorConsolePanel()
  {
//    m_infoButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/InfoButton.png");
//    m_warningButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/WarningButton.png");
//    m_errorButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/ErrorButton.png");
  }
  
  void KreatorConsolePanel::OnImguiRender(bool& isOpen)
  {
    ImGui::Begin("Log", &isOpen);
    ImGui::End();
  }
} // namespace Kreator
