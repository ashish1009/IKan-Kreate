//
//  ECS_Utils.hpp
//  IKan
//
//  Created by Ashish . on 05/08/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace IKan::ECS_Utils
{
  /// This function parent set the parent of entity
  /// - Parameters:
  ///   - scene: reference
  ///   - entity: Current entity
  ///   - parent: parent entity
  void ParentEntity(Ref<Scene> scene, Entity entity, Entity parent);
  /// This function removes the parent relation
  /// - Parameters:
  ///   - scene: reference
  ///   - entity: parent entity
  void UnparentEntity(Ref<Scene> scene, Entity entity, bool convertToWorldSpace = true);
  
  /// This function updates the transform of children entities
  /// - Parameters:
  ///   - scene: reference
  ///   - entity: parent entity
  ///   - position: position
  ///   - scale: scale
  ///   - rotation: rotation
  ///   - controllerParent: controller parent entity
  void UpdateChildrenTransform(Ref<Scene> scene, Entity entity, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Entity controllerParent);

} // namespace IKan::ECS_Utils
