//
//  AABB.cpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#include "AABB.hpp"
#include "Renderer/Renderer2D.hpp"

namespace IKan
{
  AABB::AABB()
  : min(0.0f), max(0.0f)
  {
    
  }
  
  AABB::AABB(const glm::vec3& min, const glm::vec3& max)
  : min(min), max(max)
  {
    
  }
}
