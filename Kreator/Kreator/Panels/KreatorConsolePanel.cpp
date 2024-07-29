//
//  KreatorConsolePanel.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "KreatorConsolePanel.hpp"

#include "Layers/RendererLayer.hpp"

namespace Kreator
{
#define MAX_INPUT_BUFFER_LENGTH 128
  
  static ImVec4 s_infoButtonOnTint = ImVec4(0.0f, 0.431372549f, 1.0f, 1.0f);
  static ImVec4 s_warningButtonOnTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
  static ImVec4 s_errorButtonOnTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);
  static ImVec4 s_noTint = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
  
  KreatorConsolePanel::KreatorConsolePanel()
  {
    m_infoButtonTex = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Editor/InfoButton.png");
    m_warningButtonTex = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Editor/WarningButton.png");
    m_errorButtonTex = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Editor/ErrorButton.png");
  }
  
  void KreatorConsolePanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("KreatorConsolePanel::OnImGuiRender");
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
    
    if (UI::DrawButton("Clear", UI::FontType::Bold, UI::Color::Text, UI::Color::Muted))
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
    if (UI::ImageButton(m_infoButtonTex, imageSize, ImVec4(0, 0, 0, 0), infoButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Info;
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 2) - rightSideOffset);
    if (UI::ImageButton(m_warningButtonTex, imageSize, ImVec4(0, 0, 0, 0), warningButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Warning;
    }
    
    ImGui::SameLine(ImGui::GetWindowWidth() - (buttonOffset * 1) - rightSideOffset);
    if (UI::ImageButton(m_errorButtonTex, imageSize, ImVec4(0, 0, 0, 0), errorButtonTint))
    {
      m_messageFilters ^= (int16_t)ConsoleMessage::Category::Error;
    }
  }
  
  void KreatorConsolePanel::RenderConsole()
  {
    UI::ScopedFont logFont(UI::Font::Get(UI::FontType::FixedWidth));
    ImGui::BeginChild("LogMessages");
    {
      UI::ScopedColor bgColor(ImGuiCol_ChildBg, UI::Color::Background);
      
      if (m_messageBufferBegin == 0)
      {
        m_displayMessageInspector = false;
        m_selectedMessage = nullptr;
      }
      
      if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) and !m_isMessageInspectorHovered)
      {
        m_displayMessageInspector = false;
        m_selectedMessage = nullptr;
      }
      
      for (uint32_t i = 0; i < m_messageBufferBegin; i++)
      {
        const auto& msg = m_messageBuffer[i];
        // Copying to replace the content ...
        std::string messageText = msg.GetMessage();
        
        // If search result passes
        if (s_searchFilter.PassFilter(messageText.c_str()))
        {
          if (m_messageFilters & (int16_t)msg.GetCategory())
          {
            UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(10, 5));
            // Alternate color
            if (i % 2 == 0)
            {
              ImGui::PushStyleColor(ImGuiCol_ChildBg, UI::ColorWithMultipliedValue(UI::Color::Background, 0.8f));
            }
            
            ImGui::BeginChild(i + 1, ImVec2(0, ImGui::GetFontSize() * 1.75F), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysUseWindowPadding);
            
            // Selec the message to popout
            if (ImGui::IsWindowHovered() and ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
              m_selectedMessage = &m_messageBuffer[i];
              m_displayMessageInspector = true;
            }
            
            // Right click popup
            if (ImGui::BeginPopupContextWindow())
            {
              if (ImGui::MenuItem("Copy"))
              {
                ImGui::SetClipboardText(messageText.c_str());
              }
              ImGui::EndPopup();
            }
            
            // Render Icons
            ImGuiContext& g = *GImGui;
            auto size = g.FontSize;
            if (msg.GetCategory() == ConsoleMessage::Category::Info)
            {
              UI::Image(m_infoButtonTex, ImVec2(size, size), s_infoButtonOnTint);
            }
            else if (msg.GetCategory() == ConsoleMessage::Category::Warning)
            {
              UI::Image(m_warningButtonTex, ImVec2(size, size), s_warningButtonOnTint);
            }
            else if (msg.GetCategory() == ConsoleMessage::Category::Error)
            {
              UI::Image(m_errorButtonTex, ImVec2(size, size), s_errorButtonOnTint);
            }
            
            ImGui::SameLine();
            // Add ... after 200 characters
            if (messageText.length() > 200)
            {
              size_t spacePos = messageText.find_first_of(' ', 200);
              if (spacePos != std::string::npos)
              {
                messageText.replace(spacePos, messageText.length() - 1, "...");
              }
            }
            // Print Message
            ImGui::TextUnformatted(messageText.c_str());
            
            if (m_collapseMessages and msg.GetCount() > 1)
            {
              ImGui::SameLine(ImGui::GetWindowWidth() - 30);
              ImGui::Text("%d", msg.GetCount());
            }
            
            ImGui::EndChild();
            
            // Pop Alternate color
            if (i % 2 == 0)
            {
              ImGui::PopStyleColor();
            }
          }
        }
      }
      // Audio-scroll to the bottom when a new message is added
      if (m_newMessageAdded)
      {
        ImGui::SetScrollHereY(1.0f);
        m_newMessageAdded = false;
      }
      
      if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() and !m_displayMessageInspector)
      {
        ImGui::SetScrollHereY(1.0f);
      }
      
      if (m_displayMessageInspector and m_selectedMessage != nullptr)
      {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2{ 1300, 50 });
        ImGui::Begin("Message Inspector");
        
        m_isMessageInspectorHovered = ImGui::IsWindowHovered();
        
        ImGui::PushTextWrapPos();
        const auto& msg = m_selectedMessage->GetMessage();
        ImGui::TextUnformatted(msg.c_str());
        ImGui::PopTextWrapPos();
        
        ImGui::End();
      }
      else
      {
        m_isMessageInspectorHovered = false;
      }
    }
    ImGui::EndChild();
  }
} // namespace Kreator
