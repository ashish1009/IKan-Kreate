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
#include "Asset/Asset.hpp"

namespace IKan
{
  class SceneRenderer;
  class Entity;
  
  using EntityMap = std::unordered_map<UUID, Entity>;
  static const std::string SceneExtension = ".ikscene";

  class Scene : public Asset
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);
    /// This is the default destructor of EnTT Scene
    ~Scene();

    // Runtime APIs --------------------------------------------------------------------------------------------------
    /// This functionm updates the scene for editor
    /// - Parameter ts: time step of 2 frames
    void OnUpdateEditor(TimeStep ts);
    /// This functionm updates the scene for runtime
    /// - Parameter ts: time step of 2 frames
    void OnUpdateRuntime(TimeStep ts);
    
    /// This functionm renders the scene for editor
    /// - Parameters:
    ///  - editorCamera: Editor Camera
    ///  - renderer: Scene renderer cintext
    void OnRenderEditor(const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer);
    /// This functionm renders the scene for runtime
    /// - Parameters:
    ///  - ts: time step of 2 frames
    ///  - renderer: Scene renderer cintext
    void OnRenderRuntime(TimeStep ts, const Ref<SceneRenderer> renderer);
    /// This functionm renders the scene for simulation
    /// - Parameters:
    ///  - ts: time step of 2 frames
    ///  - editorCamera: Editor Camera
    ///  - renderer: Scene renderer cintext
    void OnRenderSimulation(TimeStep ts, const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer);
    
    /// This function handle runtime start
    void OnRuntimeStart();
    /// This function handle runtime stop
    void OnRuntimeStop();
    
    /// This function handle simulation start
    void OnSimulationStart();
    /// This function handle simulation stop
    void OnSimulationStop();
    
    // Fundamentals ------------------------------------------------------------------------------------------------
    /// this function copy scene to target
    /// - Parameter target: target scene
    void CopyTo(Ref<Scene>& target);
    /// This function updates the viewport of Scene
    /// - Parameters:
    ///   - width: width of view port
    ///   - height: width of view port
    void SetViewportSize(uint32_t width, uint32_t height);
    
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
    void DestroyEntity(Entity entity);

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);
    /// This function set the selected Entity
    void SetSelectedEntity(entt::entity entity);

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;
    /// This function returns the reference of registry
    entt::registry& GetRegistry();
    /// This function returns the selected entity handle
    entt::entity GetSelectedEntity() const;
    /// This function finds the main camera entity
    Entity GetMainCameraEntity();
    
    /// This function returns entity with id as specified, or empty entity if cannot be found - caller must check
    /// - Parameter id: UUID of entity
    Entity TryGetEntityWithUUID(UUID id) const;
    /// This function return entity with id as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter id: UUID for entity
    Entity GetEntityWithUUID(UUID id) const;
    /// This function return entity with entity handle as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter entityHandle: handle for entity
    Entity GetEntityWithEntityHandle(int32_t entityHandle) const;

    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);
    
    ASSET_TYPE(Scene);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function renders the 2D Entities
    void Render2DEntities();
    /// This function renders the 3D Entities
    /// - Parameter renderer: Scene Renderer Instance
    void Render3DEntities(Ref<SceneRenderer> renderer);
    /// This function renders the debug Colliders
    /// - Parameter color: color of collider
    void RenderDebugColliders(const glm::vec4& color);
    /// This function initilzie physics world
    void InitializePhysicsWorld();
    /// This function destroy physics world
    void DestroyPhysicsWorld();

    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name = "";
    uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

    // EnTT -----------------------------
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
    
    // Entity ---------------------------
    EntityMap m_entityIDMap;
    entt::entity m_selectedEntity;
    std::function<void(Entity)> m_onEntityDestroyedCallback;

    // Physics
    reactphysics3d::PhysicsCommon m_physics3DCommon;
    reactphysics3d::PhysicsWorld* m_physics3DWorld = nullptr;

    friend class Entity;
  };
} // namespace IKan
