//
//  KreatorConsolePanel.cpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
//

#include "KreatorConsolePanel.hpp"
#include "Layer/KreatorLayer.hpp"

namespace Kreator
{
#define MAX_INPUT_BUFFER_LENGTH 128
  
  static ImVec4 s_infoButtonOnTint = ImVec4(0.0f, 0.431372549f, 1.0f, 1.0f);
  static ImVec4 s_warningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
  static ImVec4 s_errorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
  static ImVec4 s_noTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  
  KreatorConsolePanel::KreatorConsolePanel()
  {
    m_infoButtonTex = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Editor/InfoButton.png");
    m_warningButtonTex = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Editor/WarningButton.png");
    m_errorButtonTex = TextureFactory::Create(KreatorLayer::Get().GetClientResorucePath() / "Textures/Editor/ErrorButton.png");
  }
  
  void KreatorConsolePanel::OnImGuiRender(bool& isOpen)
  {
    ImGui::Begin("Log", &isOpen);
    RenderMenu();
    ImGui::Separator();
    RenderConsole();
    ImGui::End();
  }
  
  void KreatorConsolePanel::RenderMenu()
  {

  }
  
  void KreatorConsolePanel::RenderConsole()
  {

  }
} // namespace Kreator
