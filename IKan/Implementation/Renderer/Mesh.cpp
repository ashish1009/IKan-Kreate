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

} // namespace IKan
