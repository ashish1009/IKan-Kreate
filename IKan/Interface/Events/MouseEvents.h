//
//  MouseEvents.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Events/Events.h"
#include "Core/MouseCodes.h"

namespace IKan
{
  /// This class handles the events from mouse
  class MouseMovedEvent : public Event
  {
  public:
    /// This constructor creates the event for mouse
    /// - Parameters:
    ///  - xOffset: x position of Mouse:
    ///  - xOffset: y position of Mouse:
    MouseMovedEvent(float xOffset, float yOffset)
    : m_mouseX(xOffset), m_mouseY(yOffset)
    {
      
    }
    /// This function returns the X position of mouse
    float GetX() const
    {
      return m_mouseX;
    }
    /// This function returns the Y position of mouse
    float GetY() const
    {
      return m_mouseY;
    }
    
    EVENT_CLASS_TYPE(MouseMoved);
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
  private:
    float m_mouseX, m_mouseY;
  };
  
  /// This class handles the events when mouse is scrolled
  class MouseScrolledEvent : public Event
  {
  public:
    /// This constructor creates the event for mouse scroll
    /// - Parameters:
    ///  - xOffset: x Offset of Scrolling:
    ///  - yOffset: y Offset of Scrolling:
    MouseScrolledEvent(float xOffset, float yOffset)
    : m_xOffset(xOffset), m_yOffset(yOffset)
    {
      
    }
    /// This function returns x Offset of Scrolling
    float GetXOffset() const
    {
      return m_xOffset;
    }
    /// This function returns y Offset of Scrolling
    float GetYOffset() const
    {
      return m_yOffset;
    }
    
    EVENT_CLASS_TYPE(MouseScrolled);
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
  private:
    float m_xOffset, m_yOffset;
  };
  
  /// This class handles the events from mouse buttons.
  class MouseButtonEvent : public Event
  {
  public:
    /// This function returns the Mouse button pressed
    MouseButton GetMouseButton() const
    {
      return m_button;
    }
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    
  protected:
    /// This constructor creates the mouse button event instance
    /// - Parameter button: mouse button
    MouseButtonEvent(MouseButton button) : m_button(button)
    {
      
    }
    
    // Memeber variable
    MouseButton m_button;
  };
  
  /// This class handles the events when mouse button is pressed
  class MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    /// This constructor creates the mouse button pressed event instance
    /// - Parameter button: Mouse button pressed
    MouseButtonPressedEvent(MouseButton button)
    : MouseButtonEvent(button)
    {
      
    }
    
    EVENT_CLASS_TYPE(MouseButtonPressed);
  };
  
  /// This class handles the events when mouse button is released
  class MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    /// This constructor creates the mouse button released event instance
    /// - Parameter button: Mouse button released.
    MouseButtonReleasedEvent(MouseButton button)
    : MouseButtonEvent(button)
    {
      
    }
    
    EVENT_CLASS_TYPE(MouseButtonReleased);
  };
} // namespace IKan
