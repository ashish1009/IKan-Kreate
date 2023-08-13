//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

#include "Core/AABB.hpp"
#include "Asset/Asset.hpp"
#include "Renderer/Graphics/RendererBuffers.hpp"

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
    uint32_t baseVertex;
    uint32_t baseIndex;
    uint32_t materialIndex;
    uint32_t indexCount;
    uint32_t vertexCount;
    
    glm::mat4 transform{ 1.0f }; // World transform
    glm::mat4 localTransform{ 1.0f };
    AABB boundingBox;
    
    std::string nodeName, meshName;
  };
  
  /// MeshSource is a representation of an actual asset file on disk. Meshes are created from MeshSource
  class MeshSource : public Asset
  {
  public:
    /// This is the constructor to create a Mesh source from file
    /// - Parameter filename: Mesh source file path
    MeshSource(const std::string& filename);
    /// This is the constructor to create a Mesh source from vertices indices and transform
    /// - Parameters:
    ///   - vertices: vertices
    ///   - indices: indices
    ///   - transform: transform
    MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
    /// This is the constructor to create a Mesh source from vertices indices and sub meshees
    /// - Parameters:
    ///   - vertices: verices
    ///   - indices: indices
    ///   - submeshes: subm,eshed
    MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const std::vector<Submesh>& submeshes);
    virtual ~MeshSource();

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the submeshes
    std::vector<Submesh>& GetSubMeshes();
    /// This function returns the Vertex Buffer
    Ref<VertexBuffer> GetVertexBuffer();
    /// This function returns the Index Buffer
    Ref<IndexBuffer> GetIndexBuffer();
    /// This function returns the Submeshes vector
    std::vector<Submesh>& GetSubmeshes();
    /// This function returns the Submeshes vector as const
    const std::vector<Submesh>& GetSubmeshes() const;

    /// This function creates a Mesh source from file
    /// - Parameter filename: Mesh source file path
    static Ref<MeshSource> Create(const std::string& filename);
    /// This function create a Mesh source from vertices indices and transform
    /// - Parameters:
    ///   - vertices: vertices
    ///   - indices: indices
    ///   - transform: transform
    static Ref<MeshSource> Create(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                  const glm::mat4& transform);
    /// This function a Mesh source from vertices indices and sub meshees
    /// - Parameters:
    ///   - vertices: verices
    ///   - indices: indices
    ///   - submeshes: subm,eshed
    static Ref<MeshSource> Create(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                  const std::vector<Submesh>& submeshes);
    ASSET_TYPE(MeshSource);
    
  private:
    // Member Fucntions ---------------------------------------------------------------------------------------------
    void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_filePath;
    Scope<Assimp::Importer> m_importer; 
    const aiScene* m_scene;
    std::unordered_map<aiNode*, std::vector<uint32_t>> m_nodeMap;

    std::vector<Submesh> m_submeshes;
    AABB m_boundingBox;
        
    // Graphics
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;

    std::vector<Vertex> m_vertices;
    std::vector<Index> m_indices;
    std::unordered_map<uint32_t, std::vector<Triangle>> m_triangleCache;
  };
} // namespace IKan
