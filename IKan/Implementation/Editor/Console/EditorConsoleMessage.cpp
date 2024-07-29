//
//  EditorConsoleMessage.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EditorConsoleMessage.hpp"

namespace IKan
{
  ConsoleMessage::ConsoleMessage()
  : m_messageID(0), m_message(""), m_count(0), m_category(Category::None)
  {
    
  }
  
  ConsoleMessage::ConsoleMessage(const std::string& message, Category category)
  : m_messageID(std::hash<std::string>()(message)), m_message(message), m_count(1), m_category(category)
  {
    
  }
  
  uint64_t ConsoleMessage::GetMessageID() const
  {
    return m_messageID;
  }
  const std::string& ConsoleMessage::GetMessage() const
  {
    return m_message;
  }
  uint32_t ConsoleMessage::GetCount() const
  {
    return m_count;
  }
  ConsoleMessage::Category ConsoleMessage::GetCategory() const
  {
    return m_category;
  }
} // namespace IKan
