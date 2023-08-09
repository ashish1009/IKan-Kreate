//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Mesh.hpp"

namespace IKan
{
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);
 
  namespace Utils {
    
    glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix)
    {
      glm::mat4 result;
      //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
      result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
      result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
      result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
      result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
      return result;
    }
  } // namespace Utils
  
  static const uint32_t s_MeshImportFlags =
  aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
  aiProcess_Triangulate |             // Make sure we're triangles
  aiProcess_SortByPType |             // Split meshes by primitive type
  aiProcess_GenNormals |              // Make sure we have legit normals
  aiProcess_GenUVCoords |             // Convert UVs if required
  aiProcess_OptimizeMeshes |          // Batch draws where possible
  aiProcess_JoinIdenticalVertices |
  aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and
                                      // renormalise sum to 1
  aiProcess_ValidateDataStructure |   // Validation
  aiProcess_GlobalScale;              // e.g. convert cm to m for fbx import (and other formats where cm is native)
  
  Ref<MeshSource> MeshSource::Create(const std::string& filename)
  {
    return CreateRef<MeshSource>(filename);
  }
  Ref<MeshSource> MeshSource::Create(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                     const glm::mat4& transform)
  {
    return CreateRef<MeshSource>(vertices, indices, transform);
  }
  Ref<MeshSource> MeshSource::Create(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                     const std::vector<Submesh>& submeshes)
  {
    return CreateRef<MeshSource>(vertices, indices, submeshes);
  }

  MeshSource::MeshSource(const std::string& filename)
  : m_filePath(filename)
  {
    MESH_LOG("Loading Mesh : {0}", m_filePath.c_str());
    m_importer = CreateScope<Assimp::Importer>();
    const aiScene* scene = m_importer->ReadFile(filename, s_MeshImportFlags);
    
    if (!scene)
    {
      MESH_LOG("Failed to load mesh file: {0}", m_filePath);
      SetFlag(AssetFlag::Invalid);
      return;
    }
    
    m_scene = scene;
    
    // If no meshes in the scene, there's nothing more for us to do
    if (!m_scene->HasMeshes())
    {
      return;
    }

    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    
    m_boundingBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
    m_boundingBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    
    m_submeshes.reserve(scene->mNumMeshes);
    for (unsigned m = 0; m < scene->mNumMeshes; m++)
    {
      aiMesh* mesh = scene->mMeshes[m];
      
      Submesh& submesh = m_submeshes.emplace_back();
      submesh.baseVertex = vertexCount;
      submesh.baseIndex = indexCount;
      submesh.materialIndex = mesh->mMaterialIndex;
      submesh.vertexCount = mesh->mNumVertices;
      submesh.indexCount = mesh->mNumFaces * 3;
      submesh.meshName = mesh->mName.C_Str();

      vertexCount += mesh->mNumVertices;
      indexCount += submesh.indexCount;
      
      IK_ASSERT(mesh->HasPositions(), "Meshes require positions.");
      IK_ASSERT(mesh->HasNormals(), "Meshes require normals.");
      
      // Vertices
      auto& aabb = submesh.boundingBox;
      aabb.min = { FLT_MAX, FLT_MAX, FLT_MAX };
      aabb.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
      
      for (size_t i = 0; i < mesh->mNumVertices; i++)
      {
        Vertex vertex;
        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        aabb.min.x = glm::min(vertex.position.x, aabb.min.x);
        aabb.min.y = glm::min(vertex.position.y, aabb.min.y);
        aabb.min.z = glm::min(vertex.position.z, aabb.min.z);
        aabb.max.x = glm::max(vertex.position.x, aabb.max.x);
        aabb.max.y = glm::max(vertex.position.y, aabb.max.y);
        aabb.max.z = glm::max(vertex.position.z, aabb.max.z);

        if (mesh->HasTangentsAndBitangents())
        {
          vertex.tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
          vertex.binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
        }
        
        if (mesh->HasTextureCoords(0))
        {
          vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
        }
        
        m_vertices.push_back(vertex);
      }
      
      // Indices
      for (size_t i = 0; i < mesh->mNumFaces; i++)
      {
        IK_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
        Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
        m_indices.push_back(index);
        
        m_triangleCache[m].emplace_back(m_vertices[index.V1 + submesh.baseVertex],
                                        m_vertices[index.V2 + submesh.baseVertex],
                                        m_vertices[index.V3 + submesh.baseVertex]);
      }
    }
    
    TraverseNodes(scene->mRootNode);
    
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
    
    LoadGraphicsdata();
    
    m_vertexBuffer = VertexBuffer::Create(m_vertices.data(), (uint32_t)(m_vertices.size() * sizeof(Vertex)));
    m_indexBuffer = IndexBuffer::CreateWithSize(m_indices.data(), (uint32_t)(m_indices.size() * sizeof(Index)));
  }
  
  MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform)
  {
    
  }
  MeshSource::MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                         const std::vector<Submesh>& submeshes)
  {
    
  }
  MeshSource::~MeshSource()
  {
    
  }
  
  void MeshSource::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level)
  {
    glm::mat4 localTransform = Utils::Mat4FromAIMatrix4x4(node->mTransformation);
    glm::mat4 transform = parentTransform * localTransform;
    m_nodeMap[node].resize(node->mNumMeshes);
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
      uint32_t mesh = node->mMeshes[i];
      auto& submesh = m_submeshes[mesh];
      submesh.nodeName = node->mName.C_Str();
      submesh.transform = transform;
      submesh.localTransform = localTransform;
      m_nodeMap[node][i] = mesh;
    }
    
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
      TraverseNodes(node->mChildren[i], transform, level + 1);
    }
  }
} // namespace IKan
