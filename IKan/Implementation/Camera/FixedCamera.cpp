//
//  FixedCamera.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "FixedCamera.hpp"

namespace IKan
{
  void FixedCamera::SetViewport(const float width, const float height)
  {
    IK_PROFILE();
    if (0 == width or 0 == height or (width == s_viewportWidth and height == s_viewportHeight))
    {
      return;
    }
    
    s_viewportWidth = (uint32_t)width;
    s_viewportHeight = (uint32_t)height;
    s_projection = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
  }
} // namespace IKan
