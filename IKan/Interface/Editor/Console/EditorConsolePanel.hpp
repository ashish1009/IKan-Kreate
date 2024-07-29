//
//  EditorConsolePanel.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Editor/EditorPanel.hpp"
#include "Editor/Console/EditorConsoleMessage.hpp"

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
    virtual void OnImGuiRender(bool& isOpen) override = 0;
    /// Handle Scene Play
    void OnScenePlay();
    
  protected:
    static constexpr uint32_t s_messageBufferCapacity = 1000;
    
    bool m_shouldClearOnPlay = false;
    bool m_collapseMessages = false;
    bool m_newMessageAdded = false;
    
    int32_t m_messageFilters;
    uint32_t m_messageBufferBegin = 0;
    
    std::array<ConsoleMessage, s_messageBufferCapacity> m_messageBuffer;
    
  private:
    /// This function push the message for consol
    /// - Parameter message: Message
    static void PushMessage(const ConsoleMessage& message);
    
    static EditorConsolePanel* s_instance;
    friend class EditorConsoleSink;
  };
} // namespace IKan

