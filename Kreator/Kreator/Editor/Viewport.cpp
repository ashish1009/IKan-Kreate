//
//  Viewport.cpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#include "Viewport.hpp"

namespace Kreator
{
  void Viewport::GetMouseViewportSpace()
  {
    IK_PERFORMANCE("Viewport::GetMouseViewportSpace");
    auto [mx, my] = ImGui::GetMousePos();
    const auto& viewportBounds = bounds;
    mx -= viewportBounds[0].x;
    my -= viewportBounds[0].y;
    auto viewportWidth = viewportBounds[1].x - viewportBounds[0].x;
    auto viewportHeight = viewportBounds[1].y - viewportBounds[0].y;
    
    mouseViewportSpace = { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
  }
  bool Viewport::IsMouseSpaceInViewport()
  {
    return (mouseViewportSpace.spaceMouseX >= -1.0f and mouseViewportSpace.spaceMouseX <= 1.0f and
            mouseViewportSpace.spaceMouseY >= -1.0f and mouseViewportSpace.spaceMouseY <= 1.0f);
  }
} // namespace Kreator
