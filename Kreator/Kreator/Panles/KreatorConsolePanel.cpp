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
#define MAX_INPUT_BUFFER_LENGTH 128

  static ImVec4 s_infoButtonOnTint = ImVec4(0.0f, 0.431372549f, 1.0f, 1.0f);
  static ImVec4 s_warningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
  static ImVec4 s_errorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
  static ImVec4 s_noTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

  KreatorConsolePanel::KreatorConsolePanel()
  {
    m_infoButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/InfoButton.png");
    m_warningButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/WarningButton.png");
    m_errorButtonTex = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Editor/ErrorButton.png");
  }
  
  void KreatorConsolePanel::OnImguiRender(bool& isOpen)
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

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 5));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 3));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (UI::DrawRoundButton("Clear", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 10))
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
    
    if (Kreator_UI::Widgets::SearchWidget(s_searchFilter.InputBuf, MAX_INPUT_BUFFER_LENGTH, "Filter ..."))
    {
      s_searchFilter.Build();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar(2);
  }
  
  void KreatorConsolePanel::RenderConsole()
  {
  }
} // namespace Kreator
