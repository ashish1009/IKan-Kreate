//
//  Prefabs.hpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#pragma once

#include "Scene/Entity.hpp"

namespace IKan
{
  class Prefab : public Asset
  {
  public:
    /// This creates the prefab with empty entity
    Prefab();
    /// This is default destructor
    ~Prefab();
    
    /// This funcion creates prefab from entity
    /// - Note: Replaces existing entity if present
    /// - Parameters:
    ///  - entity: entity
    ///  - serialize: serialize flag
    void Create(Entity entity, bool serialize = true);

    /// This funcion creats the prefab asset
    static Ref<Prefab> Create();
    ASSET_TYPE(Prefab);

  private:
    /// This function create prefab entity
    /// - Parameter entity: entity
    Entity CreatePrefabFromEntity(Entity entity);

    Ref<Scene> m_scene;
    Entity m_entity;
    
    friend class Scene;
    friend class PrefabSerializer;
  };
} // namespace IKan
