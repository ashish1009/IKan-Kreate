//
//  Entity.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace IKan
{
  /// This class stores the entity data for scene
  class Entity
  {
  public:
    /// This is the default Contructor for Entity
    Entity() = default;
    /// This is the Scene Contructor for Entity
    /// - Parameters:
    ///   - handle: Entity Handle
    ///   - scene: Scene context
    Entity(entt::entity handle, Scene* scene);
    /// This is the Default Destructor for Entity
    ~Entity() = default;
    
  private:
    entt::entity m_entityHandle{ entt::null };
    Scene* m_scene = nullptr;
  };
} // namespace IKan
