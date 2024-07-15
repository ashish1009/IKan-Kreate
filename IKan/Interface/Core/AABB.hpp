//
//  AABB.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan
{
  /// This structure stores the AABB bounding box of entity
  struct AABB
  {
    glm::vec3 min, max;
    
    AABB() {}
    AABB(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}
  };
} // namespace IKan
