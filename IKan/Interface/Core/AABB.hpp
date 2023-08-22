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
    
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);
    void Draw() const;    
  };
} // namespace IKan
