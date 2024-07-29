//
//  EditorConsoleMessage.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace IKan
{
  /// This class store thje message to be printed on Editor console
  class ConsoleMessage
  {
  public:
    enum class Category
    {
      None = -1,
      Info = BIT(0),
      Warning = BIT(1),
      Error = BIT(2)
    };
    
  public:
    /// This is the default constructor
    ConsoleMessage();
    /// This is the Parameter constructor
    /// - Parameters:
    ///   - message: Message to be printed
    ///   - category: Message category
    ConsoleMessage(const std::string& message, Category category);
    
    /// This function returns the  message ID
    uint64_t GetMessageID() const;
    /// This function returns the  message stribng
    const std::string& GetMessage() const;
    /// This function returns the  message Count
    uint32_t GetCount() const;
    /// This function returns the  message Category
    Category GetCategory() const;
    
  private:
    uint64_t m_messageID;
    std::string m_message;
    uint32_t m_count;
    Category m_category;
    
    friend class EditorConsolePanel;
  };
} // namespace IKan

