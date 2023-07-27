//
//  CoreWindow.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This is the Interface class for creating window
  class Window
  {
  public:
    /// This structure stores the Window specificaion
    struct Specification
    {
      std::string title = "IKan Window";
      uint32_t width = 1600, height = 900;
      
      bool isFullscreen = false;
      bool hideTitleBar = false;
      
#ifndef __APPLE__
      bool isVsync = true;
#endif
    };
    
    /// This is the default virtual destructor for window
    virtual ~Window() = default;
    
    // Static Create API ---------------------------------------------------------------------------------------------
    /// This function creates the window instance based on the operating system supported by IKan Engine
    /// - Parameter specification: Window specification
    [[nodiscard]] static Ref<Window> Create(const Specification& specification);
  };
} // namespace IKan
