//
//  SceneSerializer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace YAML
{
  class Emitter;
  class Node;
} // namespace YAML

namespace IKan
{
  class SceneSerializer
  {
  public:
    /// This is Constructor creates instance of Scene serialize and store the scene as pointer
    /// - Parameter scene: scene pointer
    SceneSerializer(const Ref<Scene>& scene);
    
    /// This functions serializes(Saves) the scene at path 'file_path'
    /// - Parameter file_path: path wehre scene need to be saved
    void Serialize(const std::filesystem::path& filepath);
    /// This functions serializes(Opens) the scene frol path 'file_path'
    /// - Parameter file_path: path which need to be loaded
    bool Deserialize(const std::filesystem::path& filepath);
    
  private:
    Ref<Scene> m_scene;
  };
} // namespace IKan
