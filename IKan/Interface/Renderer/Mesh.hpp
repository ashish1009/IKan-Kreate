//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#pragma once

namespace IKan
{
  // Vertex of Static Submesh
  struct StaticVertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
    glm::vec3 tangent;
    glm::vec3 biTangent;
  };
  
  /// Index Infor of each submesh
  struct Index
  {
    uint32_t V1, V2, V3;
  };
  
  /// Triamgele coordinate
  struct Triangle
  {
    StaticVertex V0, V1, V2;
    Triangle(const StaticVertex& v0, const StaticVertex& v1, const StaticVertex& v2) : V0(v0), V1(v1), V2(v2) {}
  };

} // namespace IKan
