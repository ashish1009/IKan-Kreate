//
//  MeshFactory.hpp
//  IKan
//
//  Created by Ashish . on 21/08/23.
//

#pragma once

#include "Renderer/Mesh.hpp"

namespace IKan
{
  class MeshFactory
  {
  public:
    /// This function creates the 3D box
    /// - Parameter size: size of box
    static AssetHandle CreateBox(const glm::vec3& size);
    /// This function creates the Sphere
    /// - Parameter radius: radius of sphere
    static AssetHandle CreateSphere(float radius);
    /// This funciton creates the Capsule
    /// - Parameters:
    ///   - radius: Radius of capsule
    ///   - height: height of capsule 
    static AssetHandle CreateCapsule(float radius, float height);
  };
} // namespace IKan
