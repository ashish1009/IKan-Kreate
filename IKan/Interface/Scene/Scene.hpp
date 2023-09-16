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
#include "Physics/3D/PhysicsScene.hpp"

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
    /// This function updates the scene for editor
    /// - Parameter ts: time step of 2 frames
    void OnUpdateEditor(TimeStep ts);
    /// This functionm updates the scene for runtime
    /// - Parameter ts: time step of 2 frames
    void OnUpdateRuntime(TimeStep ts);
    
    /// This function renders the scene for editor
    /// - Parameters:
    ///  - editorCamera: Editor Camera
    ///  - renderer: Scene renderer cintext
    void OnRenderEditor(const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer);
    /// This function renders the scene for runtime
    /// - Parameters:
    ///  - ts: time step of 2 frames
    ///  - renderer: Scene renderer cintext
    void OnRenderRuntime(TimeStep ts, const Ref<SceneRenderer> renderer);
    /// This function renders the scene for simulation
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
    
    /// This function calls on scene close
    void OnClose();
    
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
    /// This function duplicate the entity
    /// - Parameter entity: entity to be duplicated
    Entity DuplicateEntity(Entity entity);

    /// This function parent set the parent of entity
    /// - Parameters:
    ///   - entity: Current entity
    ///   - parent: parent entity
    void ParentEntity(Entity entity, Entity parent);
    /// This function removes the parent relation
    /// - Parameter entity: parent entity
    void UnparentEntity(Entity entity, bool convertToWorldSpace);

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

    /// This function Updates the camera viewport
    void UpdateCamerasViewport();

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);
    /// This function set the selected Entity
    void SetSelectedEntity(entt::entity entity);
    /// This function clears the selected entity
    void ClearSelectedEntity();
    /// This function set the entity deletion callback
    /// - Parameter callback: callback funtion
    void SetEntityDestroyedCallback(const std::function<void(Entity)>& callback);
    /// This function deletes the asset handles while scene save
    /// - Parameter handle: handle
    void AddUnsavedAssetHandles(AssetHandle handle);
    /// This fuction clear all unsaved assets
    void ClearUnsavedAssets();
    
    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;
    /// This function returns the reference of registry
    entt::registry& GetRegistry();
    /// This function returns the selected entity handle
    const std::vector<entt::entity>& GetSelectedEntity() const;
    /// This function finds the main camera entity
    Entity GetMainCameraEntity();
    /// This function returns the max ID given to entity
    uint32_t GetMaxEntityId() const;

    /// This function returns entity with id as specified, or empty entity if cannot be found - caller must check
    /// - Parameter id: UUID of entity
    Entity TryGetEntityWithUUID(UUID id) const;
    /// This function return entity with id as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter id: UUID for entity
    Entity GetEntityWithUUID(UUID id) const;
    /// This function return entity with entity handle as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter entityHandle: handle for entity
    Entity GetEntityWithEntityHandle(int32_t entityHandle) const;
    
    /// This function returns the physcis settings
    PhysicsSettings& GetPhysicsSettings();
    /// This function returns the physics Debug renderer
    reactphysics3d::DebugRenderer GetPhysicsDebugRenderer() const;
    
    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);
    
    /// This funcrion returns all the components of type Components
    template<typename... Components> auto GetAllEntitiesWith()
    {
      return m_registry.view<Components...>();
    }

    ASSET_TYPE(Scene);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function renders the 2D Entities
    void Render2DEntities();
    /// This function renders the 3D Entities
    /// - Parameter renderer: Scene Renderer Instance
    void Render3DEntities(Ref<SceneRenderer> renderer);
    /// This function initilzie physics world
    void InitializePhysicsWorld();
    /// This function destroy physics world
    void DestroyPhysicsWorld();
    /// This function check entity is selected
    /// - Parameter entity: Selected Entity
    bool IsEntitySelected(entt::entity entity) const;
    
    // Registry Callbacks --------------
    void OnStaticMeshComponentConstruct(entt::registry& registry, entt::entity entity);
    void OnStaticMeshComponentDestroy(entt::registry& registry, entt::entity entity);

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
    std::vector<entt::entity> m_selectedEntities;
    std::function<void(Entity)> m_onEntityDestroyedCallback;
    
    // Project Registry Data ------------
    // Store the current asset handle if we close the scene or project without saving scene then these
    // need to remove from the Asset Regirtry
    std::stack<AssetHandle> m_unsavedAssetHandles;

    // Physics Data
    PhysicsSettings m_physicsSettings;
    Ref<PhysicsScene> m_physicsScene;
  
    friend class Entity;
    friend class SceneSerializer;
  };
} // namespace IKan
