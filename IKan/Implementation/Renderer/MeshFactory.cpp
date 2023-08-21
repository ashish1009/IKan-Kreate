//
//  MeshFactory.cpp
//  IKan
//
//  Created by Ashish . on 21/08/23.
//

#include "MeshFactory.hpp"

namespace IKan
{
  AssetHandle MeshFactory::CreateBox(const glm::vec3& size)
  {
 
    return AssetHandle();
  }
  
  AssetHandle MeshFactory::CreateSphere(float radius)
  {
    return AssetHandle();
  }
  
  static void CalculateRing(size_t segments, float radius, float y, float dy, float height, float actualRadius,
                            std::vector<StaticVertex>& vertices)
  {
  }
  
  AssetHandle MeshFactory::CreateCapsule(float radius, float height)
  {
    return AssetHandle();
  }
} // namespace IKan
