//
//  FixedCamera.cpp
//  IKan
//
//  Created by Ashish . on 14/02/24.
//

#include "FixedCamera.hpp"

namespace IKan
{
  void FixedCamera::SetViewport(float width, float height)
  {
    RETURN_IF (s_viewportWidth == width and s_viewportHeight == height)
    s_viewportWidth = width;
    s_viewportHeight = height;
    s_projection = glm::ortho( 0.0f, (float)width, 0.0f, (float)height);
  }
} // namespace IKan
