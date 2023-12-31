//
//  DefaultMeshes.hpp
//  IKan
//
//  Created by Ashish . on 08/09/23.
//

#pragma once

namespace Kreator
{
  class DefaultMesh
  {
  public:
    enum class Type
    {
      Cube, Sphere, Capsule
    };
    
    /// This function initializes the default meshes
    static void Initialize();
    /// This function destroy the default meshes
    static void Shutdown();
    
    /// This function returns the Reference of Mesh from type
    static Ref<MeshSource> GetMesh(Type type);
    
  private:
    inline static std::unordered_map<Type, Ref<MeshSource>> s_meshes;
  };
} // namespace Kreator
