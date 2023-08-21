//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Core/AABB.hpp"
#include "Renderer/Graphics/Shader.hpp"

class aiScene;
class aiNode;

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
    int32_t objectID;
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
  };

  class MeshSource : public Asset
  {
  public:
    /// This Constructor loads the mesh from assimp library and store the data
    /// - Parameters:
    ///   - filePath: mesh model file path
    ///   - entityID: Entity id
    MeshSource(const std::string& filePath, uint32_t entityID);
    /// This destructor destiory the loaded mesh and delete all the data
    ~MeshSource();
    
    /// This funciton creates the mesh from assimp library and store the data
    /// - Parameters:
    ///   - filePath: mesh model file path
    ///   - entityID: Entity id
    static Ref<MeshSource> Create(const std::string& filePath, uint32_t entityID = -1);
    
    ASSET_TYPE(MeshSource);
    
  private:
    // Member functions ----------------------------------------------------------------------------------------------
    /// This function store the certices and indices of the mesh
    void StoreVerticesAndIndices();
    /// This function rocesses a node in a recursive fashion. Processes each individual mesh located at the node and
    /// repeats this process on its children nodes (if any).
    /// - Parameters:
    ///   - node: node pointer of submesh
    ///   - parentTransform: parent node transform
    ///   - level: level
    void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

    // Member variables ----------------------------------------------------------------------------------------------
    std::string m_filePath = "";
    uint32_t m_entityID;

    // To enclose in bouding box
    AABB m_boundinBox, m_worldBoundingBox;

    std::vector<SubMesh> m_submeshes;
    std::vector<StaticVertex> m_staticVertices;
    std::vector<Index> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::unordered_map<uint32_t, std::vector<Triangle>> m_triangleCache;

    // Assimp
    const aiScene* m_scene;
    
    // TODO: Temp Later make it material
    Ref<Shader> m_shader;
  };
} // namespace IKan
