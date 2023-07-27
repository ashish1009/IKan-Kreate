//
//  KeyEvents.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"
#include "Core/KeyCodes.h"

namespace IKan
{
  /// Key base event class
  class KeyEvent : public Event
  {
  public:
    /// This is default virtual destructor for Key Event class
    virtual ~KeyEvent() = default;
    
    /// This function returns the key code of event
    Key GetKeyCode() const
    {
      return m_keyCode;
    }
    
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);
    
  protected:
    KeyEvent(Key keyCode)
    : m_keyCode(keyCode)
    {
      
    }
    
    // Member Variable
    Key m_keyCode;
  };
  
  /// Key Pressed Event class
  class KeyPressedEvent : public KeyEvent
  {
  public:
    /// Default virtual destructor
    virtual ~KeyPressedEvent() = default;
    
    /// This this Key pressed event constructor, updates the key code and repeated value
    /// - Parameters:
    ///   - key_code: Key code of event:
    ///   - repeated_count: repeat count:
    KeyPressedEvent(Key keyCode, int32_t repeatedCount)
    : KeyEvent(keyCode), m_repeatedCount(repeatedCount)
    {
      
    }
    
    /// This function returns the count of key repeated
    int32_t GetRepeatCount() const { return m_repeatedCount; }
    
    EVENT_CLASS_TYPE(KeyPressed);
    
  private:
    int32_t m_repeatedCount = 0;
  };
  
  /// Key Released event class
  class KeyReleasedEvent : public KeyEvent
  {
  public:
    /// Default virtual destructor
    virtual ~KeyReleasedEvent() = default;
    
    /// Key Release Event
    /// - Parameter key_code: Key code that released:
    KeyReleasedEvent(Key keyCode)
    : KeyEvent(keyCode)
    {
      
    }
    
    EVENT_CLASS_TYPE(KeyReleased);
  };
  
  /// Key typed Event class
  class KeyTypedEvent : public KeyEvent
  {
  public:
    /// Default virtual destructor
    virtual ~KeyTypedEvent() = default;
    
    /// Key Type event
    /// - Parameter key_code: Key code pressed:
    KeyTypedEvent(Key keyCode) : KeyEvent(keyCode)
    {
      
    }
    
    EVENT_CLASS_TYPE(KeyTyped);
  };
  
} // namespace IKan
