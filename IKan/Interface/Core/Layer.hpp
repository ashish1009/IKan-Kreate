//
//  Layer.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include "Event/Event.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This is Layer Interface class. Core Application manages Below API As per functionality defined.
  /// Client Layers should be derived from this base class
  class Layer
  {
  public:
    Layer(const std::string& name)
    : m_name(name)
    {
      
    }
    
    // Virtual APIs --------------------------------------------------------------------------------------------------
    virtual ~Layer() = default;
    
    /// This funciton initialises the layer, calls when we push layer in the application (To be called only once)
    virtual void OnAttach() = 0;
    /// This funciton destroy the layer, calls when we pop layer from the application (To be called only once)
    virtual void OnDetach() = 0;
    
    /// This function renders the GUI Window for this layer. To be called each frame from application.
    /// - Important: Core IKan::Application is taking care to call the RenderGui API for all Layers
    virtual void OnImGuiRender() = 0;
    /// This function updates the layer data. To be called each frame from application.
    /// - Parameter ts: Time step between 2 Frames
    /// - Important: Core IKan::Application is taking care to call the Update(ts) API for all Layers
    virtual void OnUpdate(TimeStep ts) = 0;
    /// This function handles all the events triggered for the layer. To be whenever an event is  triggered in the
    /// window
    /// - Parameter event: Event (Base class) intance. Dispatch event from Dispatcher
    /// - Important: Core IKan::Application is taking care to call the EventHandler(event) API for all Layers
    virtual void OnEvent(Event& event) = 0;
    
    // Getter --------------------------------------------------------------------------------------------------------
    /// This function returns the name of layer
    const std::string& GetName() const
    {
      return m_name;
    };
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Layer)
    
  private:
    std::string m_name {"Default"};
  };
} // namespace IKan
