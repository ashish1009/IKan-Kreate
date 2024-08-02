//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include <entt/entt.hpp>

#include "Assets/Asset.hpp"

namespace IKan
{
  class Entity;

  using EntityMap = std::unordered_map<UUID, Entity>;

  enum class SceneType : uint8_t
  {
    None, _2D, _3D
  };

  /// This class stores the scene data (Entity) for renderer
  class Scene : public Asset
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - sceneType: Type of scene
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(SceneType sceneType, const std::string& name, uint32_t maxEntityCapacity);
    /// This is the default destructor of EnTT Scene
    ~Scene();

    // Entity Manager ------------------------------------------------------------------------------------------------
    /// This function creates an unique entity with UUID
    /// - Parameter name: Name of entity
    [[nodiscard]] Entity CreateEntity(const std::string& name);
    /// This function creates an unique entity with UUID
    /// - Parameters:
    ///   - parent: Parent Entity Handle
    ///   - name: Name of entity
    [[nodiscard]] Entity CreateChildEntity(Entity parent, const std::string& name);
    /// This function creates an unique entity with UUID
    /// - Parameters:
    ///   - uuid: Unique ID of entity
    ///   - name: Name of entity
    [[nodiscard]] Entity CreateEntityWithID(UUID uuid, const std::string& name);

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);

    // Getters ------------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;
    /// This function returns the reference of registry
    entt::registry& GetRegistry();

    // Static APIs --------------------------------------------------------------------------------------------------
    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - sceneType: Type of scene
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(SceneType sceneType, const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);

    ASSET_TYPE(Scene);
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name {};
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};
    SceneType m_sceneType {SceneType::None};

    // EnTT -----------------------------
    entt::registry m_registry;
    uint32_t m_registryCapacity {0};
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
    EntityMap m_entityIDMap;

    friend class Entity;
  };
} // namespace IKan
