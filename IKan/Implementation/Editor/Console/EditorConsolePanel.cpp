//
//  EditorConsolePanel.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EditorConsolePanel.hpp"

namespace IKan
{
  EditorConsolePanel* EditorConsolePanel::s_instance = nullptr;
  
  EditorConsolePanel::EditorConsolePanel()
  {
    IK_LOG_TRACE(LogModule::EditorConsolePanel, "Initialising Editor Console Panel Textures");
    IK_ASSERT(s_instance == nullptr);
    s_instance = this;
    m_messageFilters = (int16_t)ConsoleMessage::Category::Info | (int16_t)ConsoleMessage::Category::Warning | (int16_t)ConsoleMessage::Category::Error;
  }
  
  EditorConsolePanel::~EditorConsolePanel()
  {
    s_instance = nullptr;
  }
  
  void EditorConsolePanel::PushMessage(const ConsoleMessage& message)
  {
    if ((s_instance == nullptr) or (message.GetCategory() == ConsoleMessage::Category::None))
    {
      return;
    }
    
    if (s_instance->m_collapseMessages)
    {
      for (auto& other : s_instance->m_messageBuffer)
      {
        if (message.GetMessageID() == other.GetMessageID())
        {
          other.m_count++;
          return;
        }
      }
    }
    
    s_instance->m_messageBuffer[s_instance->m_messageBufferBegin++] = message;
    s_instance->m_newMessageAdded = true;
    
    if (s_instance->m_messageBufferBegin == s_messageBufferCapacity)
    {
      s_instance->m_messageBufferBegin = 0;
    }
  }
  
  void EditorConsolePanel::OnScenePlay()
  {
    if (m_shouldClearOnPlay)
    {
      m_messageBufferBegin = 0;
    }
  }
} // namespace IKan
