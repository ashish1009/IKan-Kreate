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
    IK_PROFILE();
    IK_LOG_TRACE("Default Mesh", "Creating Default Meshes");
    s_meshes[Type::Cube] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Cube.fbx"));
    s_meshes[Type::Sphere] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Sphere.fbx"));
    s_meshes[Type::Capsule] = MeshSource::Create(Project::GetActive()->GetMeshPath("Default/Capsule.fbx"));
  }
  
  void DefaultMesh::Shutdown()
  {
    IK_PROFILE();
    s_meshes.clear();
    IK_LOG_WARN("Default Mesh", "Destroying Default Meshes");
  }
  
  Ref<MeshSource> DefaultMesh::GetMesh(Type type)
  {
    return s_meshes.at(type);
  }
} // namespace Kreator
