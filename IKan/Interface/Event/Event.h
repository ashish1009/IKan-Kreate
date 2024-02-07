//
//  Event.h
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

namespace IKan
{
#define EVENT_CLASS_TYPE(type) \
static Event::Type GetStaticType() { \
return Event::Type::type; \
} \
virtual Event::Type GetType() const override { \
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
  
  /// This is the base class for Events. Event is registered in window event callbacks.
  /// Events can be dispached using Event Dispatcher
  class Event
  {
  public:
    /// Type of Event supported by iKan Engine
    enum class Type : uint8_t
    {
      NoEventType,
      WindowClose, WindowResize,
      KeyPressed, KeyReleased, KeyTyped,
      MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };
    
    /// Catergosy bits of Events
    enum Category : uint8_t {
      NoEventCategory = 0,
      EventCategoryApplication    = BIT(0),
      EventCategoryInput          = BIT(1),
      EventCategoryKeyboard       = BIT(2),
      EventCategoryMouse          = BIT(3),
      EventCategoryMouseButton    = BIT(4)
    };
    
    /// This is default virtual destructor for base event class.
    virtual ~Event() = default;
    
    /// This function returns the type of triggered Event. e.g. Type::WindowClosed...
    virtual Type GetType() const = 0;
    /// This function returns the Name of Event in char pointer (Convert the type of Event in const char*)
    virtual const char* GetName() const = 0;
    /// This function returns the Caregory Flag of Event as Bit poisition defined in enum Category
    virtual int32_t GetCategoryFlags() const = 0;
    
    /// This function checks if Event is aa same category as argument 'category'
    /// - Parameter - Event category type:
    bool IsInCategory(Category category) const
    {
      return GetCategoryFlags() & category;
    }
    
  public:
    bool m_isHandled = false;
  };
  
  /// This class is for dispatching the events dynamically at runtime. Event type to be checked with current event of
  /// Dispatcher and performs the functionality passed as function pointer
  class EventDispatcher
  {
  public:
    /// Constructor that store the event reference in the dispatcher
    ///  - Parameter event: Event reference triggered from window callback:
    EventDispatcher(Event& event)
    : m_event(event)
    {
      
    }
    /// This is default Event dispatcher destructor
    ~EventDispatcher() = default;
    
    /// This function Dispatches the event dynamically
    ///  - Once an event callback triggered, it check the triggered event's type( with base calss 'Even') with the
    ///    explicit event type 'T'. If same then executes  the Function F (Function pointer passed to Dispatcher)
    ///  - Parameter func: Function pointer to be triggered when even dispatches:
    ///  - Important: Event Should be Dispached using this function only. All Event handler takes only base Instance of
    ///               Event. This Dispatcher dispatches the function binded to a specific Event. So checked all the
    ///               triggered events at same time and return the calling on Function Binded as Funciton pointer <F>
    template<typename T, typename F> bool Dispatch(const F& func)
    {
      if (m_event.GetType() == T::GetStaticType())
      {
        m_event.m_isHandled |= func(static_cast<T&>(m_event));
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
