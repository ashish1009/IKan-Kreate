//
//  ApplicationEvents.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Events/Events.h"

namespace IKan
{
  /// This class handles the event to resize the window
  class WindowResizeEvent : public Event
  {
  public:
    /// This Constructs the event that set the new size of window
    /// - Parameters:
    ///  - width: width of new window:
    ///  - height: height of new window:
    WindowResizeEvent(uint32_t width, uint32_t height)
    : m_prevWidth(m_width), m_prevHeight(m_height), m_width(width), m_height(height)
    {
      
    }

    /// This function returns the previous width of window
    uint32_t GetPrevWidth() const
    {
      return m_prevWidth;
    }
    /// This function returns the previous height of window
    uint32_t GetPrevHeight() const
    {
      return m_prevHeight;
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
    uint32_t m_prevWidth, m_prevHeight;
  };
  
  /// This class handles the event to close the window
  class WindowCloseEvent : public Event
  {
  public:
    EVENT_CLASS_TYPE(WindowClose);
    EVENT_CLASS_CATEGORY(EventCategoryApplication);
  };
  
  /// This class handles the event to minimize the window
  class WindowMinimizeEvent : public Event
  {
  public:
    WindowMinimizeEvent(bool minimized)
    : m_minimized(minimized)
    {
      
    }
    
    /// This function returns if window is minimized
    bool IsMinimized() const
    {
      return m_minimized;
    }
    
    EVENT_CLASS_TYPE(WindowMinimize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    
  private:
    bool m_minimized = false;
  };
  
  /// This class handles the event to maximize the window
  class WindowMaximizeEvent : public Event
  {
  public:
    WindowMaximizeEvent(bool maximized)
    : m_maximized(maximized) 
    {
      
    }
    
    /// This function returns if window is maximized
    bool IsMaximized() const
    {
      return m_maximized;
    }
    
    EVENT_CLASS_TYPE(WindowMaximize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    
  private:
    bool m_maximized = false;
  };
} // namespace IKan
