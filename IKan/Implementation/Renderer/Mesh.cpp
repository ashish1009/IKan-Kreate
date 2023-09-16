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
#include "Asset/AssetManager.hpp"
#include "Project/Project.hpp"

namespace IKan
{
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);

  namespace MeshUtils
  {
    /// return glm mat4 from aiMatrix
    /// @param matrix ai Matrix
    static glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
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

  static constexpr aiTextureType s_AIMaterialType[MaxPBRTextureSupported] =
  {
    aiTextureType_DIFFUSE,
    aiTextureType_NORMALS,
    aiTextureType_SHININESS,
    aiTextureType_METALNESS
  };

  static const std::string s_TextureName[MaxPBRTextureSupported] =
  {
    "Albedo", "Normal", "Roughness", "Metallic"
  };

  Ref<MeshSource> MeshSource::Create(const std::string &filePath)
  {
    return CreateRef<MeshSource>(filePath);
  }
  
  Ref<MeshSource> MeshSource::Create(const std::vector<StaticVertex>& vertices, const std::vector<Index>& indices,
                                     const glm::mat4& transform)
  {
    return CreateRef<MeshSource>(vertices, indices, transform);
  }
  
  MeshSource::MeshSource(const std::string &filePath)
  : m_filePath(filePath)
  {
    MESH_LOG("Loading mesh from file {0}", m_filePath.c_str());
    
    // read file via ASSIMP
    Assimp::Importer importer;
    m_scene = importer.ReadFile(m_filePath, s_MeshImportFlags);
    
    // check for errors
    IK_ASSERT(!(!m_scene or m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE or !m_scene->mRootNode), importer.GetErrorString());
    
    StoreVerticesAndIndices();
    TraverseNodes(m_scene->mRootNode);
    LoadGraphicsdata();
    UploadMaterial();
  }
  
  MeshSource::MeshSource(const std::vector<StaticVertex>& vertices, const std::vector<Index>& indices,
                         const glm::mat4& transform)
  {
    m_staticVertices = vertices;
    m_indices = indices;
    
    SubMesh submesh;
    submesh.baseVertex = 0;
    submesh.baseIndex = 0;
    submesh.indexCount = (uint32_t)indices.size() * 3u;
    submesh.transform = transform;
    m_submeshes.push_back(submesh);

    LoadGraphicsdata();
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
        
        m_staticVertices.push_back(vertex);
      } // for (size_t i = 0; i < mesh->mNumVertices; i++)
      
      // Indices
      numFaces += mesh->mNumFaces;
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
    } // for (size_t m = 0; m < m_scene->mNumMeshes; m++)
    
    MESH_LOG("  Number of Num Meshes        | {0}", m_scene->mNumMeshes);
    MESH_LOG("  Number of Static Vertices   | {0}", m_staticVertices.size());
    MESH_LOG("  Number of Indices           | {0}", m_indices.size());
    MESH_LOG("  Number of Faces             | {0}", numFaces);
  }
  
  void MeshSource::TraverseNodes(aiNode *node, const glm::mat4 &parentTransform, uint32_t level)
  {
    glm::mat4 transform = parentTransform * MeshUtils::Mat4FromAssimpMat4(node->mTransformation);
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
      uint32_t mesh = node->mMeshes[i];
      auto& submesh = m_submeshes[mesh];
      submesh.nodeName = node->mName.C_Str();
      submesh.transform = transform;
    }
    
    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
      TraverseNodes(node->mChildren[i], transform, level + 1);
    }
  }
  
  void MeshSource::LoadGraphicsdata()
  {
    m_vertexBuffer = VertexBuffer::Create((void*)(m_staticVertices.data()),
                                          uint32_t(m_staticVertices.size() * sizeof(StaticVertex)));
    
    // Create Pipeline specification
    Pipeline::Specification pipelineSpec;
    pipelineSpec.debugName = Utils::String::GetFileNameFromPath(m_filePath);
    pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/PBR_StaticShader.glsl"));

    pipelineSpec.vertexLayout =
    {
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Normal",    ShaderDataType::Float3 },
      { "a_TexCoord",  ShaderDataType::Float2 },
      { "a_Tangent",   ShaderDataType::Float3 },
      { "a_Bitangent", ShaderDataType::Float3 },
    };
    
    // Create the Pipeline instnace
    m_pipeline = Pipeline::Create(pipelineSpec);
    m_indexBuffer = IndexBuffer::CreateWithSize((void*)m_indices.data(), uint32_t(m_indices.size() * sizeof(Index)));
    
    // Create Base Material
    m_baseMaterial = Material::Create(pipelineSpec.shader, "PBR_Static");
  }
  
  void MeshSource::UploadMaterial()
  {
    // Create the Instance to store the materials
    m_materials = CreateRef<MaterialTable>();
    
    // If mesh do not have material then return
    if (!m_scene->HasMaterials())
    {
      return;
    }
    
    MESH_LOG("  Loading Materials for Mesh");
    MESH_LOG("    Number of Materials | {0}", m_scene->mNumMaterials);
        
    for (uint32_t materialIdx = 0; materialIdx < m_scene->mNumMaterials; materialIdx++)
    {
      auto aiMaterial = m_scene->mMaterials[materialIdx];
      auto aiMaterialName = aiMaterial->GetName();
      
      MESH_LOG("    Name      | {0}", aiMaterialName.data);
      MESH_LOG("    Index     | {0}", materialIdx);
      
      aiString aiTexPath;
      aiColor3D aiColor;
      
      aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
      
      float shininess, metalness;
      if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
      {
        shininess = 80.0f; // Default value
      }
      
      if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
      {
        metalness = 0.0f;
      }
      
      float roughness = 1.0f - glm::sqrt(shininess / 100.0f);
      
      MESH_LOG("    Color     | {0} | {1} | {2}", aiColor.r, aiColor.g, aiColor.b);
      MESH_LOG("    Metalness | {0}", metalness);
      MESH_LOG("    Roughness | {0}", roughness);
      
      // Set the uniforms in materials
      glm::vec3 color = {aiColor.r, aiColor.g, aiColor.b};
      m_baseMaterial->Set("u_Material_AlbedoColor", color);
      m_baseMaterial->Set("u_Material_Metalness", metalness);
      m_baseMaterial->Set("u_Material_Roughness", roughness);

      MESH_LOG("      Uploading Textures");
      for (size_t textureIdx = 0; textureIdx < MaxPBRTextureSupported; textureIdx++)
      {
        // Load each texture
        if (aiMaterial->GetTexture(s_AIMaterialType[textureIdx], 0, &aiTexPath) == AI_SUCCESS)
        {
          std::filesystem::path path = m_filePath;
          auto parentPath = path.parent_path();
          
          std::string tempPathString = std::string(aiTexPath.data);
          std::replace( tempPathString.begin(), tempPathString.end(), '\\', '/');
          
          parentPath /= tempPathString;
          std::string texturePath = parentPath.string();
          
          MESH_LOG("      Name | {0}", s_TextureName[textureIdx]);
          MESH_LOG("      Path | {0}", texturePath);
          
          auto texture = Image::Create(texturePath);
          auto textureShaderName = "u_" + s_TextureName[textureIdx] + "Texture";
          
          m_baseMaterial->Set(textureShaderName, texture);
          m_baseMaterial->Set(textureShaderName + "Toggle", 1.0f);
        } // If mesh exist in assim model
      } // For each texture indexbb
    }
  }
  
  void MeshSource::SerializeMaterials()
  {
    m_materials->Serialize();
  }
  const std::vector<SubMesh>& MeshSource::GetSubMeshes() const
  {
    return m_submeshes;
  }
  const Ref<Pipeline>& MeshSource::GetPipeline() const
  {
    return m_pipeline;
  }
  const Ref<Material>& MeshSource::GetBaseMaterial() const
  {
    if (m_materialIndex >= 0)
    {
      return m_materials->GetMaterialAssets().at(m_materialIndex)->GetMaterial();
    }
    else
    {
      return m_baseMaterial;
    }
  }
  const std::vector<Triangle>& MeshSource::GetTriangleCache(uint32_t submeshIndex) const
  {
    return m_triangleCache.at(submeshIndex);
  }
  const std::vector<glm::vec3>& MeshSource::GetVertices() const
  {
    return m_vertices;
  }
  const std::vector<Index>& MeshSource::GetIndices() const
  {
    return m_indices;
  }
  const Ref<MaterialTable>& MeshSource::GetMaterialTable() const
  {
    return m_materials;
  }
  uint32_t MeshSource::GetMaterialIndex() const
  {
    return m_materialIndex;
  }

} // namespace IKan
