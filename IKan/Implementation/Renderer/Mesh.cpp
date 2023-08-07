//
//  Mesh.cpp
//  IKan
//
//  Created by Ashish . on 07/08/23.
//

#include "Mesh.hpp"

namespace IKan
{
  MeshSource::MeshSource(const std::string& filename)
  {
    
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
  
  
  Ref<MeshSource> MeshSource::Cerate(const std::string& filename)
  {
    return CreateRef<MeshSource>(filename);
  }
  Ref<MeshSource> MeshSource::Cerate(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                     const glm::mat4& transform)
  {
    return CreateRef<MeshSource>(vertices, indices, transform);
  }
  Ref<MeshSource> MeshSource::Cerate(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
                                     const std::vector<Submesh>& submeshes)
  {
    return CreateRef<MeshSource>(vertices, indices, submeshes);
  }

} // namespace IKan
