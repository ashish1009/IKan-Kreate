//
//  Ray.cpp
//  IKan
//
//  Created by Ashish . on 05/08/24.
//

#include "Ray.hpp"

namespace IKan
{
  Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
  {
    Origin = origin;
    Direction = direction;
  }
  
  Ray Ray::Zero()
  {
    return { {0.0f, 0.0f, 0.0f},{0.0f, 0.0f, 0.0f} };
  }
  
  bool Ray::IntersectsAABB(const AABB& aabb, float& t) const
  {
    IK_PROFILE();
    glm::vec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / Direction.x;
    dirfrac.y = 1.0f / Direction.y;
    dirfrac.z = 1.0f / Direction.z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    const glm::vec3& lb = aabb.min;
    const glm::vec3& rt = aabb.max;
    float t1 = (lb.x - Origin.x) * dirfrac.x;
    float t2 = (rt.x - Origin.x) * dirfrac.x;
    float t3 = (lb.y - Origin.y) * dirfrac.y;
    float t4 = (rt.y - Origin.y) * dirfrac.y;
    float t5 = (lb.z - Origin.z) * dirfrac.z;
    float t6 = (rt.z - Origin.z) * dirfrac.z;
    
    float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));
    
    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
      t = tmax;
      return false;
    }
    
    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
      t = tmax;
      return false;
    }
    
    t = tmin;
    return true;
  }
  
  bool Ray::IntersectsTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& t) const
  {
    IK_PROFILE();
    const glm::vec3 E1 = b - a;
    const glm::vec3 E2 = c - a;
    const glm::vec3 N = cross(E1, E2);
    const float det = -glm::dot(Direction, N);
    const float invdet = 1.f / det;
    const glm::vec3 AO = Origin - a;
    const glm::vec3 DAO = glm::cross(AO, Direction);
    const float u = glm::dot(E2, DAO) * invdet;
    const float v = -glm::dot(E1, DAO) * invdet;
    t = glm::dot(AO, N) * invdet;
    return (det >= 1e-6f and t >= 0.0f and u >= 0.0f and v >= 0.0f and (u + v) <= 1.0f);
  }
} // namespace IKan
