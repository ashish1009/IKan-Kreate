//
//  Events.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan
{
  // API to bind class member function to std function pointer
#define IK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

  /// This enum stores the type of events supported by IKan engine
  enum class EventType : uint8_t
  {
    NoEventType,
    WindowClose, WindowResize, WindowMinimize, WindowMaximize,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
  };
  
  /// This enum stores the category of events
  enum EventCategory : uint8_t
  {
    NoEventCategory = 0,
    EventCategoryApplication    = BIT(0),
    EventCategoryInput          = BIT(1),
    EventCategoryKeyboard       = BIT(2),
    EventCategoryMouse          = BIT(3),
    EventCategoryMouseButton    = BIT(4)
  };
  
  /// This class is the base class for triggered events in Window. Stores the API to check the dispatched event type
  class Event
  {
  public:
    // Virtual APIs --------------------------------------------------------------------------------------------------
    /// This function returns the type of triggered Event. e.g. EventType::WindowClosed...
    virtual EventType GetType() const = 0;
    /// This function returns the name of Event in char pointer (Convert the type of event in const char*)
    virtual const char* GetName() const = 0;
    /// This function returns the caregory flag of event as bit poisition defined in enum category
    virtual int32_t GetCategoryFlags() const = 0;
    
    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function checks if event is same as category as argument 'category'
    /// - Parameter - Event category type:
    bool IsInCategory(EventCategory category) const
    {
      return GetCategoryFlags() & category;
    }
    
  public:
    bool isHandled {false};
  };
} // namespace IKan
