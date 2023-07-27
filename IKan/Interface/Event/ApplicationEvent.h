//
//  ApplicationEvent.h
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"

namespace IKan
{
  /// This class handles the event to resize the window
  class WindowResizeEvent : public Event {
  public:
    /// This is the virtual Default Destructor for Window Resize Event
    virtual ~WindowResizeEvent() = default;
    
    /// This Constructs the event that set the new size of window
    /// - Parameters:
    ///  - width: width of new window:
    ///  - height: height of new window:
    WindowResizeEvent(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
    {
      
    }
    /// This function returns the width of window
    uint32_t GetWidth() const
    {
      return m_width;
    }
    /// This function returns the height of window
    uint32_t GetHeight() const
    {
      return m_height;
    }
    
    EVENT_CLASS_TYPE(WindowResize);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
    
  private:
    uint32_t m_width, m_height;
  };
  
  /// Window Close event
  class WindowCloseEvent : public Event {
  public:
    /// This is the virtual Default Destructor for Window ResCloseize Event
    virtual ~WindowCloseEvent() = default;
    
    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
  };
  
} // namespace IKan
