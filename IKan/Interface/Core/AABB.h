//
//  AABB.h
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#pragma once

namespace IKan
{
  struct AABB
  {
    glm::vec3 min, max;
    
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);
  };
} // namespace IKan
