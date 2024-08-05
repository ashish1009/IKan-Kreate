//
//  Viewport.hpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

namespace Kreator
{
  /// This structure store the kretor viewport data
  struct Viewport
  {
    /// This structure stores the mouse space in viewport
    struct MouseViewportSpace
    {
      float spaceMouseX, spaceMouseY;
    };

    bool isHovered {false};
    bool isFocused {false};
    
    uint32_t width {0}, height {0};
    
    glm::vec2 bounds[2];
    MouseViewportSpace mouseViewportSpace;
    
    /// This function returns the space mouse position
    void GetMouseViewportSpace();
    /// This function check the mouse position inside viewport
    bool IsMouseSpaceInViewport();
  };
} // namespace Kreator
