//
//  Window.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include "Event/Event.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This structure storest the window specificaion for GLFW Window creation
  struct WindowSpecification
  {
    std::string title = "Untitled Window";
    uint32_t width = 0, height = 0;
    bool isFullScreen = false;
    bool hideTitleBar = false;
#ifndef __APPLE__
    bool isVsync = true;
#endif
  };
} // namespace IKan
