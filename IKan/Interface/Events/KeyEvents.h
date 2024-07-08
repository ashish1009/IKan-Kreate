//
//  KeyEvents.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Events/Events.h"
#include "Core/KeyCodes.h"

namespace IKan
{
  /// This class handles the events from keyboard
  class KeyEvent : public Event
  {
  public:
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
  
  /// This class handles the events from when key button is pressed
  class KeyPressedEvent : public KeyEvent
  {
  public:
    /// This constructor creates key pressed instances, updates the key code and repeated value
    /// - Parameters:
    ///   - keyCode: Key code of event:
    ///   - repeatedCount: repeat count:
    KeyPressedEvent(Key keyCode, int32_t repeatedCount)
    : KeyEvent(keyCode), m_repeatedCount(repeatedCount)
    {
      
    }
    
    /// This function returns the count of key repeated
    int32_t GetRepeatCount() const 
    {
      return m_repeatedCount;
    }
    
    EVENT_CLASS_TYPE(KeyPressed);
    
  private:
    int32_t m_repeatedCount = 0;
  };
  
  /// This class handles the events from when key button is released
  class KeyReleasedEvent : public KeyEvent
  {
  public:
    /// This constructor creates key release instances, updates the key code
    /// - Parameter keyCode: Key code that released:
    KeyReleasedEvent(Key keyCode)
    : KeyEvent(keyCode)
    {
      
    }
    
    EVENT_CLASS_TYPE(KeyReleased);
  };
  
  /// This class handles the events from when key button is typed
  class KeyTypedEvent : public KeyEvent
  {
  public:
    /// This constructor creates key typed instances, updates the key code
    /// - Parameter keyCode: Key code pressed:
    KeyTypedEvent(Key keyCode) : KeyEvent(keyCode)
    {
      
    }
    
    EVENT_CLASS_TYPE(KeyTyped);
  };
} // namespace IKan
