//
//  KreatorUtils.h
//  Kreator
//
//  Created by Ashish . on 01/03/24.
//

#pragma once

// Kreator Layer ------------------------------------------------------------------------------------------------
namespace Kreator::KreatorUtils
{
  static auto CheckRayPassMesh = [](AssetHandle meshHandle, Entity entity, const glm::vec3& origin, const glm::vec3& direction) -> float {
    const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
    if (!mesh or mesh->IsFlagSet(AssetFlag::Missing))
    {
      return -1;
    }
    
    const glm::mat4& transform = entity.GetTransform().Transform();
    auto& submeshes = mesh->GetSubMeshes();
    for (uint32_t i = 0; i < submeshes.size(); i++)
    {
      const auto& submesh = submeshes[i];
      Ray ray =
      {
        glm::inverse(transform * submesh.transform) * glm::vec4(origin, 1.0f),
        glm::inverse(glm::mat3(transform * submesh.transform)) * direction
      };
      
      float distance;
      if (ray.IntersectsAABB(submesh.boundingBox, distance))
      {
        const auto& triangleCache = mesh->GetTriangleCache(i);
        for (const auto& triangle : triangleCache)
        {
          if (ray.IntersectsTriangle(triangle.V0.position, triangle.V1.position, triangle.V2.position, distance))
          {
            return distance;
          }
        } // For each triangle cache
      } // Bounding box intersect
    } // Each Submesh
    return -1;
  };
} // namespace Kreator::KreatorUtils
