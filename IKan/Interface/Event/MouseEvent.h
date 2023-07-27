//
//  MouseEvent.h
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"
#include "Core/MouseCodes.h"

namespace IKan
{
  /// Mouse base Event class
  class MouseMovedEvent : public Event
  {
  public:
    /// Default virtual destructor
    virtual ~MouseMovedEvent() = default;
    
    /// Mouse moved event
    /// - Parameters:
    ///  - x_offset: x position of Mouse:
    ///  - y_offset: y position of Mouse:
    MouseMovedEvent(float x, float y)
    : m_mouseX(x), m_mouseY(y)
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
  
  /// Mouse scrol event
  class MouseScrolledEvent : public Event
  {
  public:
    /// Default virtual destructor
    virtual ~MouseScrolledEvent() = default;
    
    /// Mosue Scroll Event
    /// - Parameters:
    ///  - x_offset: x Offset of Scrolling:
    ///  - y_offset: y Offset of Scrolling:
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
  
  /// Mouse button base calss
  class MouseButtonEvent : public Event
  {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonEvent() = default;
    
    /// This function returns the Mouse button pressed
    MouseButton GetMouseButton() const
    {
      return m_button;
    }
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    
  protected:
    MouseButtonEvent(MouseButton button) : m_button(button) {}
    MouseButton m_button;
  };
  
  /// Mouse button pressed
  class MouseButtonPressedEvent : public MouseButtonEvent
  {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonPressedEvent() = default;
    
    /// Mouse button pressed Event
    /// - Parameter button: Mouse button pressed:
    MouseButtonPressedEvent(MouseButton button)
    : MouseButtonEvent(button)
    {
      
    }
    
    EVENT_CLASS_TYPE(MouseButtonPressed);
  };
  
  /// Mouse button released
  class MouseButtonReleasedEvent : public MouseButtonEvent
  {
  public:
    /// Default virtual destructor
    virtual ~MouseButtonReleasedEvent() = default;
    
    /// Mouse button release ebent
    /// - Parameter button: Mouse button released:
    MouseButtonReleasedEvent(MouseButton button)
    : MouseButtonEvent(button)
    {
      
    }
    
    EVENT_CLASS_TYPE(MouseButtonReleased);
  };
  
} // namespace IKan
