//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

#include "Core/AABB.hpp"

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp
{
  class Importer;
}

namespace IKan
{
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 binormal;
    glm::vec2 texcoord;
  };
  
  struct Index
  {
    uint32_t V1, V2, V3;
  };
  
  static_assert(sizeof(Index) == 3 * sizeof(uint32_t));
  
  struct Triangle
  {
    Vertex V0, V1, V2;
    
    Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
    : V0(v0), V1(v1), V2(v2) {}
  };
  
  class Submesh
  {
  public:
    uint32_t BaseVertex;
    uint32_t BaseIndex;
    uint32_t MaterialIndex;
    uint32_t IndexCount;
    uint32_t VertexCount;
    
    glm::mat4 Transform{ 1.0f }; // World transform
    glm::mat4 LocalTransform{ 1.0f };
    AABB BoundingBox;
    
    std::string NodeName, MeshName;
  };
  
  /// MeshSource is a representation of an actual asset file on disk. Meshes are created from MeshSource
  class MeshSource : public Asset
  {
    
  };
} // namespace IKan
