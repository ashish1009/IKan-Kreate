//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

// Assimp Files
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Mesh.hpp"

namespace IKan
{
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);

  static const uint32_t s_MeshImportFlags =
  aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
  aiProcess_Triangulate |             // Make sure we're triangles
  aiProcess_SortByPType |             // Split meshes by primitive type
  aiProcess_GenNormals |              // Make sure we have legit normals
  aiProcess_GenUVCoords |             // Convert UVs if required
  aiProcess_OptimizeMeshes |          // Batch draws where possible
  aiProcess_ValidateDataStructure;    // Validation

  Ref<MeshSource> MeshSource::Create(const std::string &filePath, uint32_t entityID)
  {
    return CreateRef<MeshSource>(filePath, entityID);
  }
  
  MeshSource::MeshSource(const std::string &filePath, uint32_t entityID)
  : m_filePath(filePath), m_entityID(entityID)
  {
    MESH_LOG("Loading mesh from file {0}", m_filePath.c_str());
    
    // read file via ASSIMP
    Assimp::Importer importer;
    m_scene = importer.ReadFile(m_filePath, s_MeshImportFlags);
    
    // check for errors
    IK_ASSERT(!(!m_scene or m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode), importer.GetErrorString());
    
    StoreVerticesAndIndices();
  }
  
  MeshSource::~MeshSource()
  {
    MESH_LOG("Destroying mesh from file {0}", m_filePath.c_str());
  }
  
  void MeshSource::StoreVerticesAndIndices()
  {
    MESH_LOG("  Number of Submesh           | {0}",  m_scene->mNumMeshes);
    
    m_boundinBox.min = { FLT_MAX, FLT_MAX, FLT_MAX };
    m_boundinBox.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;
    m_submeshes.reserve(m_scene->mNumMeshes);
    
    // Travers all the sub meshes from the scene and add vertex and indices for
    // each submesh. Store them in class
    for (size_t m = 0; m < m_scene->mNumMeshes; m++)
    {
      aiMesh* mesh = m_scene->mMeshes[m];
      
      // Store the data in submesh
      SubMesh& submesh = m_submeshes.emplace_back();
      submesh.baseVertex = vertexCount;
      submesh.baseIndex = indexCount;
      submesh.materialIndex = mesh->mMaterialIndex;
      submesh.indexCount = mesh->mNumFaces * 3;
      submesh.meshName = mesh->mName.C_Str();
      
      vertexCount += mesh->mNumVertices;
      indexCount += submesh.indexCount;
      
      IK_ASSERT(mesh->HasPositions(), "Meshes require positions.");
      IK_ASSERT(mesh->HasNormals(), "Meshes require normals.");
      
      // Store the bounding box of submesh
      auto& aabb = submesh.boundingBox;
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
        
        vertex.objectID = (int32_t)m_entityID;
        m_staticVertices.push_back(vertex);
      } // for (size_t i = 0; i < mesh->mNumVertices; i++)
      
      // Indices
      for (size_t i = 0; i < mesh->mNumFaces; i++)
      {
        IK_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
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
    } // for (size_t m = 0; m < scene_->mNumMeshes; m++)
    
    MESH_LOG("  Number of Static Vertices   | {0}", m_staticVertices.size());
    MESH_LOG("  Number of Indices           | {0}", m_indices.size());

  }
} // namespace IKan
