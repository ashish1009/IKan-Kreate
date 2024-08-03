//
//  EntitySerializer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace IKan
{
  class Entity;
  class Scene;
  
  /// This class serialzes the entity
  class EntitySerializer
  {
  public:
    /// This function serialises the entity
    /// - Parameters:
    ///   - out: yamal out
    ///   - entity: entity handle
    static void SerializeEntity(YAML::Emitter& out, Entity entity);
    /// This function deserialises the entity
    /// - Parameters:
    ///   - scene: Scene context
    ///   - entitiesNode: yamal root node
    static void DeserializeEntities(Ref<Scene> scene, YAML::Node& entitiesNode);
  };
} // namespace IKan
