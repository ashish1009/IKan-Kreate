//
//  FixedCamera.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "FixedCamera.hpp"

namespace IKan
{
  void FixedCamera::SetViewport(float width, float height)
  {
    if (s_viewportWidth == width and s_viewportHeight == height)
    {
      return;
    }

    s_viewportWidth = width;
    s_viewportHeight = height;
    s_projection = glm::ortho( 0.0f, (float)width, 0.0f, (float)height);
  }
} // namespace IKan
