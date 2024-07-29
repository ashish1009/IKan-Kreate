//
//  EditorConsoleSink.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EditorConsoleSink.hpp"
#include "Editor/Console/EditorConsolePanel.hpp"

namespace IKan
{
  EditorConsoleSink::EditorConsoleSink(uint32_t bufferCapacity)
  : m_messageBufferCapacity(bufferCapacity), m_messageBuffer(bufferCapacity)
  {
    
  }
  
  void EditorConsoleSink::sink_it_(const spdlog::details::log_msg& msg)
  {
    spdlog::memory_buf_t formatted;
    spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
    m_messageBuffer[m_messageCount++] = ConsoleMessage(fmt::to_string(formatted), GetMessageCategory(msg.level));
    
    if (m_messageCount == m_messageBufferCapacity)
    {
#if 0
      flush_();
#endif
    }
  }
  
  void EditorConsoleSink::flush_()
  {
    for (const auto& message : m_messageBuffer)
    {
      if (message.GetCategory() == ConsoleMessage::Category::None)
      {
        continue;
      }
      // PUSH Message in Editor Console
      EditorConsolePanel::PushMessage(message);
    }
    
    m_messageCount = 0;
  }
  
  ConsoleMessage::Category EditorConsoleSink::GetMessageCategory(spdlog::level::level_enum level)
  {
    switch (level)
    {
      case spdlog::level::trace:
      case spdlog::level::debug:
      case spdlog::level::info:
        return ConsoleMessage::Category::Info;
      case spdlog::level::warn:
        return ConsoleMessage::Category::Warning;
      case spdlog::level::err:
      case spdlog::level::critical:
        return ConsoleMessage::Category::Error;
      default:
        break;
    }
    
    IK_ASSERT("Invalid Message Category!");
    return ConsoleMessage::Category::None;
  }
} // namespace IKan
