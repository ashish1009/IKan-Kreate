//
//  cpp
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
  
  void AABB::Draw() const
  {
    // Left Face
    Renderer2D::DrawLine({min.x, min.y, min.z}, {min.x, max.y, min.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, min.y, min.z}, {min.x, min.y, max.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, max.y, min.z}, {min.x, max.y, max.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, min.y, max.z}, {min.x, max.y, max.z}, {1, 1, 1, 1});
    
    // Right Face
    Renderer2D::DrawLine({max.x, min.y, min.z}, {max.x, max.y, min.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({max.x, min.y, min.z}, {max.x, min.y, max.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({max.x, max.y, min.z}, {max.x, max.y, max.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({max.x, min.y, max.z}, {max.x, max.y, max.z}, {1, 1, 1, 1});
    
    // Joints
    Renderer2D::DrawLine({min.x, min.y, min.z}, {max.x, min.y, min.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, max.y, min.z}, {max.x, max.y, min.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, min.y, max.z}, {max.x, min.y, max.z}, {1, 1, 1, 1});
    Renderer2D::DrawLine({min.x, max.y, max.z}, {max.x, max.y, max.z}, {1, 1, 1, 1});
  }
}
