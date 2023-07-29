//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <entt/entt.hpp>
#include "Core/TimeStep.hpp"
#include "Camera/EditorCamera.hpp"

namespace IKan
{
  class Entity;
  using EntityMap = std::unordered_map<UUID, Entity>;

  class Scene
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(uint32_t maxEntityCapacity = 200000);
    /// This is the default destructor of EnTT Scene
    ~Scene();
    
    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(uint32_t maxEntityCapacity = 200000);
    
    // Entity Manager ------------------------------------------------------------------------------------------------
    /// This function creates an unique entity with UUID
    /// - Parameter name: Name of entity
    [[nodiscard]] Entity CreateEntity(const std::string& name = "");
    /// This function creates an unique entity with UUID
    /// - Parameters:
    ///   - parent: Parent Entity Handle
    ///   - name: Name of entity
    [[nodiscard]] Entity CreateChildEntity(Entity parent, const std::string& name = "");
    /// This function creates an unique entity with UUID
    /// - Parameters:
    ///   - uuid: Unique ID of entity
    ///   - name: Name of entity
    [[nodiscard]] Entity CreateEntityWithID(UUID uuid, const std::string& name = "");

    // This function returns entity with id as specified, or empty entity if cannot be found - caller must check
    Entity TryGetEntityWithUUID(UUID id) const;

  private:
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
    EntityMap m_entityIDMap;

    friend class Entity;
  };
} // namespace IKan
