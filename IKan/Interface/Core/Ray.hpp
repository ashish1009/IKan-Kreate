//
//  Ray.hpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#pragma once

#include "Core/AABB.h"

namespace IKan
{
  struct Ray
  {
    glm::vec3 Origin, Direction;
    
    /// This is the constructor for Ray
    /// - Parameters:
    ///   - origin: Ray origin
    ///   - direction: Ray Direction
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    /// This function Intersect the ray to AABB
    /// - Parameters:
    ///   - aabb: AABB
    ///   - t: Ditance
    bool IntersectsAABB(const AABB& aabb, float& t) const;
    /// This function intersect the ray with Triangle
    /// - Parameters:
    ///   - a: Trinagle point A
    ///   - b: Trinagle point B
    ///   - c: Trinagle point C
    ///   - t: Distance
    bool IntersectsTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t) const;
    static Ray Zero();
  };
} // namespace IKan
