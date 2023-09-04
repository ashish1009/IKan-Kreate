//
//  SceneSerializer.hpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
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
    void Serialize(const std::string& filepath);
    /// This functions serializes(Opens) the scene frol path 'file_path'
    /// - Parameter file_path: path which need to be loaded
    bool Deserialize(const std::string& filepath);
    
  private:
    Ref<Scene> m_scene;
  };
} // namespace IKan
