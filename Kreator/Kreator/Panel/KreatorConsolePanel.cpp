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
    ImVec4 infoButtonTint = (m_messageFilters & (int16_t)ConsoleMessage::Category::Info) ? s_infoButtonOnTint : s_noTint;
    ImVec4 warningButtonTint = (m_messageFilters & (int16_t)ConsoleMessage::Category::Warning) ? s_warningButtonOnTint : s_noTint;
    ImVec4 errorButtonTint = (m_messageFilters & (int16_t)ConsoleMessage::Category::Error) ? s_errorButtonOnTint : s_noTint;

    UI::ScopedColor button(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    UI::ScopedColor buttonActive(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5, 3));
    UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 5);

    if (UI::DrawRoundButton("Clear", UI::Color::Muted, 5))
    {
      m_messageBufferBegin = 0;
    }
    
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::TextUnformatted("Clear On Play:");
    ImGui::SameLine();
    ImGui::Checkbox("##ClearOnPlay", &m_shouldClearOnPlay);
    
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::TextUnformatted("Collapse:");
    ImGui::SameLine();
    ImGui::Checkbox("##CollapseMessages", &m_collapseMessages);
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(MAX_INPUT_BUFFER_LENGTH);

    if (UI::Widgets::SearchWidget(s_searchFilter.InputBuf, MAX_INPUT_BUFFER_LENGTH, "Filter ...", nullptr, 5))
    {
      s_searchFilter.Build();
    }
    
    constexpr float buttonOffset = 39;
    constexpr float rightSideOffset = 15;
    ImVec2 imageSize = {15.0f, 15.0f};
    ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 3) - rightSideOffset);
    if (UI::ImageButton(m_infoButtonTex, imageSize, ImVec2(0, 1), ImVec2(1, 0), -1, ImVec4(0, 0, 0, 0), infoButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Info;
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
    if (UI::ImageButton(m_warningButtonTex, imageSize, ImVec2(0, 1), ImVec2(1, 0), -1, ImVec4(0, 0, 0, 0), warningButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Warning;
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
    if (UI::ImageButton(m_errorButtonTex, imageSize, ImVec2(0, 1), ImVec2(1, 0), -1, ImVec4(0, 0, 0, 0), errorButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Error;
    }
  }
  
  void KreatorConsolePanel::RenderConsole()
  {

  }
} // namespace Kreator
