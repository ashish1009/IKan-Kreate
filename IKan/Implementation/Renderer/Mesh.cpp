//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#include "Mesh.hpp"

namespace IKan
{
#define MESH_LOG(...) IK_LOG_TRACE(LogModule::Mesh, __VA_ARGS__);
  
  MeshSource::MeshSource(const std::string& filename)
  : m_filePath(filename)
  {
    MESH_LOG("Loading Mesh : {0}", m_filePath.c_str());
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
