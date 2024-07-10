//
//  Events.h
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan
{
#define EVENT_CLASS_TYPE(type) \
static EventType GetStaticType() { \
return EventType::type; \
} \
virtual EventType GetType() const override { \
return GetStaticType(); \
} \
virtual const char* GetName() const override { \
return #type; \
}
  
#define EVENT_CLASS_CATEGORY(category) \
virtual int32_t GetCategoryFlags() const override { \
return category; \
}

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
    /// Default virtual destructor
    virtual ~Event() = default;
    
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
  
  /// This class dispatches the event handler function based on the triggered event in window. Registers the callback
  /// function to be called on event trigger.
  class EventDispatcher
  {
  public:
    /// This constructor creates the event dispatcher instance, stores the event reference in the dispatcher
    ///  - Parameter event: Event reference triggered from window callback.
    EventDispatcher(Event& event)
    : m_event(event)
    {
      
    }
    
    /// This function Dispatches the event dynamically
    ///  - Once an event callback is triggered, it checks the triggered event's type (base class 'Event') with the
    ///    explicit event type 'T'. If same then executes the Function F (Function pointer passed to Dispatcher)
    ///  - Parameter func: Function pointer to be triggered when even dispatches.
    ///  - Important: Event Should be Dispached using this function only. All Event handler takes only base Instance of
    ///               Event. This Dispatcher dispatches the function binded to a specific Event. So checked all the
    ///               triggered events at same time and return the calling on Function Binded as Funciton pointer <F>
    template<typename T, typename F> bool Dispatch(const F& func)
    {
      if (m_event.GetType() == T::GetStaticType())
      {
        m_event.isHandled |= func(static_cast<T&>(m_event));
        return true;
      }
      return false;
    }
    
    DELETE_COPY_MOVE_CONSTRUCTORS(EventDispatcher);

  private:
    Event& m_event;
  };
  
  // Using Typedefs
  using EventCallbackFn = std::function<void(Event&)>;
} // namespace IKan
