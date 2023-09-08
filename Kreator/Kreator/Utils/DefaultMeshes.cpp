//
//  DefaultMeshes.cpp
//  IKan
//
//  Created by Ashish . on 08/09/23.
//

#include "DefaultMeshes.hpp"

namespace Kreator
{
  void DefaultMesh::Initialize()
  {
    s_meshes[Type::Cube] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Cube.fbx"));
    s_meshes[Type::Sphere] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Sphere.fbx"));
    s_meshes[Type::Capsule] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Capsule.fbx"));
  }
  
  void DefaultMesh::Shutdown()
  {
    s_meshes.clear();
  }
  
  Ref<MeshSource> DefaultMesh::GetMesh(Type type)
  {
    return s_meshes.at(type);
  }
} // namespace Kreator
