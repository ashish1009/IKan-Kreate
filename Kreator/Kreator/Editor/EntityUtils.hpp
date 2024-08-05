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
} // namespace Kreator::ECS_Utils
