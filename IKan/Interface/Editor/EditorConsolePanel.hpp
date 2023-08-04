//
//  EditorConsolePanel.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

#include "Editor/EditorPanel.hpp"
#include "Editor/EditorConsoleMessage.hpp"

namespace IKan
{
  /// This class creates panel for editor consol
  /// - Note: Once this instance is created, all the logs to be pushed in panel also
  ///         Shows only last 1000 Messages
  class EditorConsolePanel : public EditorPanel
  {
  public:
    /// Default Constructor
    EditorConsolePanel();
    /// Default Destructor
    virtual ~EditorConsolePanel();
 
    /// @see Editor Panel
    virtual void OnImguiRender(bool& isOpen) override = 0;
    
  private:
    /// This function push the message for consol
    /// - Parameter message: Message
    static void PushMessage(const ConsoleMessage& message);

    // Member Variables ----------------------------------------------------------------------------------------------
    static constexpr uint32_t s_messageBufferCapacity = 1000;

    bool m_collapseMessages = false;
    bool m_newMessageAdded = false;

    std::array<ConsoleMessage, s_messageBufferCapacity> m_messageBuffer;
    uint32_t m_messageBufferBegin = 0;

    static EditorConsolePanel* s_instance;
    friend class EditorConsoleSink;
  };
} // namespace IKan
