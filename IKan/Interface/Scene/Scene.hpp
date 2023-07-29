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
#include "Scene/Component.hpp"

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

    /// This function destriy the entity
    /// - Parameter entity: entity handle
    void DestroyEntity(Entity entity, bool excludeChildren = false, bool first = true);
    /// This function duplicate the entity
    /// - Parameter entity: entity to be duplicated
    Entity DuplicateEntity(Entity entity);

    // This function returns entity with id as specified, or empty entity if cannot be found - caller must check
    Entity TryGetEntityWithUUID(UUID id) const;

    /// This function parent set the parent of entity
    /// - Parameters:
    ///   - entity: Current entity
    ///   - parent: parent entity
    void ParentEntity(Entity entity, Entity parent);
    /// This function removes the parent relation
    /// - Parameter entity: parent entity
    void UnparentEntity(Entity entity, bool convertToWorldSpace = true);

    /// This function convert the entity to local space
    /// - Parameter entity: Entity handle
    void ConvertToLocalSpace(Entity entity);
    /// This function convert the entity to world space
    /// - Parameter entity: Entity handle
    void ConvertToWorldSpace(Entity entity);
    
    /// This function return the world transform from entity
    /// - Parameter entity: enitty handle
    glm::mat4 GetWorldSpaceTransformMatrix(Entity entity);
    /// This function return the world transform component
    /// - Parameter entity: enitty handle
    TransformComponent GetWorldSpaceTransform(Entity entity);

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function set the entity deletion callback
    /// - Parameter callback: callback funtion
    void SetEntityDestroyedCallback(const std::function<void(Entity)>& callback);

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function return entity with id as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter id: UUID for entity
    Entity GetEntityWithUUID(UUID id) const;
    /// This function return entity with entity handle as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter entityHandle: handle for entity
    Entity GetEntityWithEntityHandle(int32_t entityHandle) const;

    /// This function returns the max ID given to entity
    uint32_t GetMaxEntityId() const;

    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(uint32_t maxEntityCapacity = 200000);
    
    /// This funcrion returns all the components of type Components
    template<typename... Components> auto GetAllEntitiesWith()
    {
      return m_registry.view<Components...>();
    }

  private:
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
    EntityMap m_entityIDMap;

    std::function<void(Entity)> m_onEntityDestroyedCallback;

    friend class Entity;
  };
} // namespace IKan
