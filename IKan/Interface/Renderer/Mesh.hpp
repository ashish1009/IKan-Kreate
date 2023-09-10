//
//  Mesh.hpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#pragma once

#include "Asset/Asset.hpp"
#include "Core/AABB.hpp"
#include "Renderer/Material.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/RendererBuffers.hpp"

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
    uint32_t vertexCount;
    
    AABB boundingBox;
    glm::mat4 transform;
    
    std::string nodeName, meshName;
  };

  class MeshSource : public Asset
  {
  public:
    /// This Constructor loads the mesh from assimp library and store the data
    /// - Parameter filePath: mesh model file path
    MeshSource(const std::string& filePath);
    /// This Constructur creates the mesh source fromn vertices and indices and transform
    /// - Parameters:
    ///   - vertices: veritices
    ///   - indices: indices
    ///   - transform: transform
    MeshSource(const std::vector<StaticVertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
    /// This destructor destiory the loaded mesh and delete all the data
    virtual ~MeshSource();
    
    /// This function returns the submeshes
    const std::vector<SubMesh>& GetSubMeshes() const;
    /// This function returns the Vertices
    const std::vector<glm::vec3>& GetVertices() const;
    /// This function returns the Indices
    const std::vector<Index>& GetIndices() const;
    /// This function returns the submeshes
    /// - Parameter submeshIndex: This function returns the triangle cache
    const std::vector<Triangle>& GetTriangleCache(uint32_t submeshIndex) const;
    /// This function returns the mesh pipeline
    const Ref<Pipeline>& GetPipeline() const;
    /// This function returns the Base Material
    const Ref<Material>& GetBaseMaterial() const;

    /// This funciton creates the mesh from assimp library and store the data
    /// - Parameter filePath: mesh model file path
    static Ref<MeshSource> Create(const std::string& filePath);
    /// This function creates the mesh source fromn vertices and indices and transform
    /// - Parameters:
    ///   - vertices: veritices
    ///   - indices: indices
    ///   - transform: transform
    static Ref<MeshSource> Create(const std::vector<StaticVertex>& vertices, const std::vector<Index>& indices,
                                  const glm::mat4& transform);

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
    /// This function loads the graphics data in renderer buffers
    void LoadGraphicsdata();

    // Member variables ----------------------------------------------------------------------------------------------
    std::string m_filePath = "";

    // To enclose in bouding box
    AABB m_boundinBox, m_worldBoundingBox;

    std::vector<SubMesh> m_submeshes;
    std::vector<StaticVertex> m_staticVertices;
    std::vector<Index> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::unordered_map<uint32_t, std::vector<Triangle>> m_triangleCache;

    // Graphics
    Ref<Pipeline> m_pipeline;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
    Ref<Material> m_baseMaterial;

    // Assimp
    const aiScene* m_scene;
    
    friend class StaticMesh;
  };
} // namespace IKan
