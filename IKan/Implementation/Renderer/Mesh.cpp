//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

// Assimp Files
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "Mesh.hpp"

namespace IKan
{
#define MESH_INFO(...) IK_LOG_INFO(LogModule::Mesh, __VA_ARGS__);
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);

  namespace MeshUtils
  {
    /// return glm mat4 from aiMatrix
    /// @param matrix ai Matrix
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
  
  Ref<Mesh> Mesh::Create(const std::string &filePath)
  {
    return CreateRef<Mesh>(filePath);
  }

  Mesh::Mesh(const std::string &filePath)
  : m_filePath(filePath)
  {
    IK_PROFILE();
    MESH_INFO("Loading mesh from file {0}", m_filePath.c_str());
    
    // read file via ASSIMP
    Assimp::Importer importer;
    m_scene = importer.ReadFile(m_filePath, s_MeshImportFlags);
    
    // check for errors
    IK_ASSERT(!(!m_scene or m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode), importer.GetErrorString());
  }
  
  Mesh::~Mesh()
  {
    IK_PROFILE();
    MESH_INFO("Destroying mesh from file {0}", m_filePath.c_str());
  }
} // namespace IKan
