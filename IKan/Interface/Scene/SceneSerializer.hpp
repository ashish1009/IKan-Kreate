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
    
    /// This function serialises the entity
    /// - Parameters:
    ///   - out: yamal out
    ///   - entity: entity handle
    static void SerializeEntity(YAML::Emitter& out, Entity entity);
    /// This function deserialises the entity
    /// - Parameters:
    ///   - entitiesNode: yamal root node
    ///   - scene: Scene context
    static void DeserializeEntities(YAML::Node& entitiesNode, Ref<Scene> scene);
    
  private:
    Ref<Scene> m_scene;
  };
} // namespace IKan
