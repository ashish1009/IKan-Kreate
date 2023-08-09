//
//  AABB.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

namespace IKan
{
  struct AABB
  {
    glm::vec3 min, max;
    
    AABB()
    : min(0.0f), max(0.0f) {}
    
    AABB(const glm::vec3& min, const glm::vec3& max)
    : min(min), max(max) {}
    
  };
} // namespace IKan
