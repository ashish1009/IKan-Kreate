//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Core/AABB.hpp"

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
    int32_t object_id;
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

  /// Stores submesh content
  struct SubMesh {
    uint32_t baseVertex;
    uint32_t baseIndex;
    uint32_t materialIndex;
    uint32_t indexCount;
    
    AABB boundingBox;
    glm::mat4 transform;
    
    std::string nodeName, meshName;
    
    SubMesh() = default;
    
    DEFINE_COPY_MOVE_CONSTRUCTORS(SubMesh)
  };

  class MeshSource : public Asset
  {
  public:
    /// This Constructor loads the mesh from assim library and store the data
    /// - Parameters:
    ///   - filePath: mesh model file path
    ///   - entityID: Entity id
    MeshSource(const std::string& filePath, uint32_t entityID);
    /// This destructor destiory the loaded mesh and delete all the data
    ~MeshSource();
    
    ASSET_TYPE(MeshSource);
  };
} // namespace IKan
