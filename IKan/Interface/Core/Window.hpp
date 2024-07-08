//
//  Window.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Events/Events.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This structure stores the window specification data
  struct WindowSpecification
  {
    std::string_view title {"IKan"};
    uint32_t width {1600}, height {900};
    bool isFullScreen {false};
    bool hideTitleBar {false};
    bool startMaximized {false};
    bool resizable {true};
  };

  /// This class is the abstract for GLFW window. Provides the APIs for using GLFW window data
  /// - Note: Implementation depends on supported platform
  class Window
  {
  public:
  };
  
  /// This structure stores the API to create the window instance based on the supported platform
  struct WindowFactory
  {
    /// This function creates the window instance based on the window specification data and supported platform
    /// - Parameter specification: window specification data
    [[nodiscard("Created window should not be discarded")]] static Scope<Window> Create(const WindowSpecification& specification);
  };
} // namespace IKan
