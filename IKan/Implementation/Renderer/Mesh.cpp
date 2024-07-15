//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "Mesh.hpp"

// Assimp Files
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace IKan
{
#define MESH_INFO(...) IK_LOG_INFO(LogModule::Mesh, __VA_ARGS__);
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);
  
  namespace MeshUtils
  {
    /// return glm mat4 from aiMatrix
    /// - Parameter matrix: ai matrix
    [[maybe_unused]] static glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
    {
      glm::mat4 result;
      //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
      result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
      result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
      result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
      result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
      return result;
    }
  } // namespace MeshUtils
  
  static const uint32_t s_MeshImportFlags =
  aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
  aiProcess_Triangulate |             // Make sure we're triangles
  aiProcess_SortByPType |             // Split meshes by primitive type
  aiProcess_GenNormals |              // Make sure we have legit normals
  aiProcess_GenUVCoords |             // Convert UVs if required
  aiProcess_OptimizeMeshes |          // Batch draws where possible
  aiProcess_ValidateDataStructure;    // Validation
  
  Ref<Mesh> Mesh::Create(const std::filesystem::path &filePath)
  {
    return CreateRef<Mesh>(filePath);
  }
  
  Mesh::Mesh(const std::filesystem::path& filePath)
  : m_filePath(filePath)
  {
    IK_PROFILE();
    MESH_INFO("Loading mesh from file {0}", m_filePath.c_str());
    
    // read file via ASSIMP
    Assimp::Importer importer;
    m_scene = importer.ReadFile(m_filePath, s_MeshImportFlags);
    
    // check for errors
    IK_ASSERT(!(!m_scene or m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode), importer.GetErrorString());
    
    StoreVerticesAndIndices();
    TraverseNodes(m_scene->mRootNode);
    LoadGraphicsdata();
    
    for (const auto& submesh : m_submeshes)
    {
      AABB transformedSubmeshAABB = submesh.boundingBox;
      glm::vec3 min = glm::vec3(submesh.transform * glm::vec4(transformedSubmeshAABB.min, 1.0f));
      glm::vec3 max = glm::vec3(submesh.transform * glm::vec4(transformedSubmeshAABB.max, 1.0f));
      
      m_boundingBox.min.x = glm::min(m_boundingBox.min.x, min.x);
      m_boundingBox.min.y = glm::min(m_boundingBox.min.y, min.y);
      m_boundingBox.min.z = glm::min(m_boundingBox.min.z, min.z);
      m_boundingBox.max.x = glm::max(m_boundingBox.max.x, max.x);
      m_boundingBox.max.y = glm::max(m_boundingBox.max.y, max.y);
      m_boundingBox.max.z = glm::max(m_boundingBox.max.z, max.z);
    }
  }
  
  Mesh::~Mesh()
  {
    IK_PROFILE();
    MESH_INFO("Destroying mesh from file {0}", m_filePath.c_str());
  }
  
  void Mesh::StoreVerticesAndIndices()
  {
    IK_PROFILE();
    MESH_LOG("  Number of Submesh           | {0}",  m_scene->mNumMeshes);
    
    m_boundingBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
    m_boundingBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    m_submeshes.reserve(m_scene->mNumMeshes);
    
    // Travers all the sub meshes from the scene and add vertex and indices for
    // each submesh. Store them in class
    uint32_t numFaces = 0; // Only for Debug Log
    for (size_t m = 0; m < m_scene->mNumMeshes; m++)
    {
      aiMesh* mesh = m_scene->mMeshes[m];
      
      // Store the data in submesh
      SubMesh& submesh = m_submeshes.emplace_back();
      submesh.baseVertex = vertexCount;
      submesh.baseIndex = indexCount;
      submesh.materialIndex = mesh->mMaterialIndex;
      submesh.indexCount = mesh->mNumFaces * 3;
      submesh.vertexCount = mesh->mNumVertices;
      submesh.meshName = mesh->mName.C_Str();
      
      vertexCount += mesh->mNumVertices;
      indexCount += submesh.indexCount;
      
      IK_ASSERT(mesh->HasPositions(), "Meshes require positions.");
      IK_ASSERT(mesh->HasNormals(), "Meshes require normals.");
      
      // Store the bounding box of submesh
      AABB& aabb = submesh.boundingBox;
      aabb.min = { FLT_MAX, FLT_MAX, FLT_MAX };
      aabb.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
      
      for (size_t i = 0; i < mesh->mNumVertices; i++)
      {
        StaticVertex vertex;
        vertex.position =
        {
          mesh->mVertices[i].x,
          mesh->mVertices[i].y,
          mesh->mVertices[i].z
        };
        
        m_vertices.push_back(vertex.position);
        
        vertex.normal =
        {
          mesh->mNormals[i].x,
          mesh->mNormals[i].y,
          mesh->mNormals[i].z
        };
        
        aabb.min.x = glm::min(vertex.position.x, aabb.min.x);
        aabb.min.y = glm::min(vertex.position.y, aabb.min.y);
        aabb.min.z = glm::min(vertex.position.z, aabb.min.z);
        aabb.max.x = glm::max(vertex.position.x, aabb.max.x);
        aabb.max.y = glm::max(vertex.position.y, aabb.max.y);
        aabb.max.z = glm::max(vertex.position.z, aabb.max.z);
        
        if (mesh->HasTangentsAndBitangents())
        {
          vertex.tangent =
          {
            mesh->mTangents[i].x,
            mesh->mTangents[i].y,
            mesh->mTangents[i].z
          };
          vertex.biTangent =
          {
            mesh->mBitangents[i].x,
            mesh->mBitangents[i].y,
            mesh->mBitangents[i].z
          };
        }
        
        if (mesh->HasTextureCoords(0))
        {
          vertex.textureCoords =
          {
            mesh->mTextureCoords[0][i].x,
            mesh->mTextureCoords[0][i].y
          };
        }
        
        m_staticVertices.push_back(vertex);
      } // for (size_t i = 0; i < mesh->mNumVertices; i++)
      
      // Indices
      numFaces += mesh->mNumFaces;
      for (size_t i = 0; i < mesh->mNumFaces; i++)
      {
        IK_ASSERT(3 == mesh->mFaces[i].mNumIndices, "Must have 3 indices.");
        Index index =
        {
          mesh->mFaces[i].mIndices[0],
          mesh->mFaces[i].mIndices[1],
          mesh->mFaces[i].mIndices[2]
        };
        m_indices.push_back(index);
        
        m_triangleCache[(uint32_t)m].emplace_back(m_staticVertices[index.V1 + submesh.baseVertex],
                                                  m_staticVertices[index.V2 + submesh.baseVertex],
                                                  m_staticVertices[index.V3 + submesh.baseVertex]);
      }
    } // for (size_t m = 0; m < m_scene->mNumMeshes; m++)
    
    MESH_LOG("  Number of Num Meshes        | {0}", m_scene->mNumMeshes);
    MESH_LOG("  Number of Static Vertices   | {0}", m_staticVertices.size());
    MESH_LOG("  Number of Indices           | {0}", m_indices.size());
    MESH_LOG("  Number of Faces             | {0}", numFaces);
  }
  
  void Mesh::TraverseNodes(aiNode *node, const glm::mat4 &parentTransform, uint32_t level)
  {
    glm::mat4 transform = parentTransform * MeshUtils::Mat4FromAssimpMat4(node->mTransformation);
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
      uint32_t mesh = node->mMeshes[i];
      SubMesh& submesh = m_submeshes[mesh];
      submesh.nodeName = node->mName.C_Str();
      submesh.transform = transform;
    }
    
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
      TraverseNodes(node->mChildren[i], transform, level + 1);
    }
  }
  
  void Mesh::LoadGraphicsdata()
  {
    IK_PROFILE();
    // Create Vertex Buffer
    m_vertexBuffer = VertexBufferFactory::Create((void*)(m_staticVertices.data()),
                                                 uint32_t(m_staticVertices.size() * sizeof(StaticVertex)));
    
    // Create Pipeline specification
    PipelineSpecification pipelineSpec;
    pipelineSpec.debugName = m_filePath.filename();
    
    pipelineSpec.vertexLayout =
    {
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Normal",    ShaderDataType::Float3 },
      { "a_TexCoord",  ShaderDataType::Float2 },
      { "a_Tangent",   ShaderDataType::Float3 },
      { "a_Bitangent", ShaderDataType::Float3 },
    };
    
    // Create the Pipeline instnace
    m_pipeline = PipelineFactory::Create(pipelineSpec);
    m_indexBuffer = IndexBufferFactory::CreateWithSize((void*)m_indices.data(), uint32_t(m_indices.size() * sizeof(Index)));
  }
  const std::vector<SubMesh>& Mesh::GetSubMeshes() const
  {
    return m_submeshes;
  }
  const Ref<Pipeline>& Mesh::GetPipeline() const
  {
    return m_pipeline;
  }
  const std::vector<Triangle>& Mesh::GetTriangleCache(uint32_t submeshIndex) const
  {
    return m_triangleCache.at(submeshIndex);
  }
  const std::vector<glm::vec3>& Mesh::GetVertices() const
  {
    return m_vertices;
  }
  const std::vector<Index>& Mesh::GetIndices() const
  {
    return m_indices;
  }
} // namespace IKan
