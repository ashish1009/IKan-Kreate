//
//  ApplicationImpl.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include "Core/Application.hpp"
#include "Core/Layer.hpp"
#include "Event/Event.h"

namespace IKan
{
  /// This is the Imple class for Core application
  class Application::Impl
  {
  public:
    // Application APIs -------------------------------------------------
    /// @see Application
    void Run();
    /// @see Application
    void HandleEvents(Event& event);
    /// @see Application
    void Close();
    
    /// @see Application
    void PushLayer(const Ref<Layer>& layer);
    /// @see Application
    void PopLayer(const Ref<Layer>& layer);
  };
} // namespace IKan
