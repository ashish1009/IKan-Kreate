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
    bool isHovered {false};
    bool isFocused {false};
    
    uint32_t width {0}, height {0};
  };
} // namespace Kreator
