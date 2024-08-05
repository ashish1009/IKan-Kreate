//
//  EntityUtils.hpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#pragma once

namespace Kreator::ECS_Utils
{
  /// This funcion Draw the menu to create new entity
  /// - Parameters:
  ///   - scene: scene context
  ///   - parent: parent entity
  Entity DrawCreateEntityMenu(Ref<Scene> scene, Entity parent);
  /// This function updates the transform of children entities
  /// - Parameters:
  ///   - scene: scene context
  ///   - entity: parent entity
  ///   - position: position
  ///   - scale: scale
  ///   - rotation: rotation
  void UpdateChildrenTransform(Ref<Scene> scene, Entity entity, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Entity controllerParent);
} // namespace Kreator::ECS_Utils
