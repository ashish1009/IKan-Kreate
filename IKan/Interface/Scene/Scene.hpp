//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

#include <entt/entt.hpp>

#include "Asset/Asset.hpp"
#include "Core/TimeStep.hpp"
#include "Camera/EditorCamera.hpp"
#include "Renderer/SceneRenderer.hpp"
#include "Physics/PhysicsScene.hpp"

namespace IKan
{
  class Entity;
  class TransformComponent;

  using EntityMap = std::unordered_map<UUID, Entity>;

  class Scene : public Asset
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(const std::string& name, uint32_t maxEntityCapacity);
    /// This is the default destructor of EnTT Scene
    ~Scene();
    
    // Runtime APIs ------------------------------------------------------------------------------------------------
    /// This functionm updates the scene for editor
    void OnUpdateEditor();
    /// This functionm updates the scene for runtime
    /// - Parameter ts: time step of 2 frames
    void OnUpdateRuntime(TimeStep ts);
    
    /// This dunction handles the events of the scene
    /// - Parameter event: event triggered
    void OnRuntimeEventHandler(Event& event);
    
    /// This function renders the scene for editor
    /// - Parameters:
    ///  - editorCamera: Editor Camera
    ///  - renderer: Scene renderer cintext
    void OnRenderEditor(const EditorCamera& editorCamera, SceneRenderer& renderer);
    /// This function renders the scene for runtime
    /// - Parameters:
    ///  - ts: time step of 2 frames
    ///  - renderer: Scene renderer cintext
    void OnRenderRuntime(TimeStep ts, SceneRenderer& renderer);
    /// This function renders the scene for simulation
    /// - Parameters:
    ///  - ts: time step of 2 frames
    ///  - editorCamera: Editor Camera
    ///  - renderer: Scene renderer cintext
    void OnRenderSimulation(TimeStep ts, const EditorCamera& editorCamera, SceneRenderer& renderer);
    
    /// This function handle runtime start
    void OnRuntimeStart();
    /// This function handle runtime stop
    void OnRuntimeStop();
    
    /// This function handle simulation start
    void OnSimulationStart();
    /// This function handle simulation stop
    void OnSimulationStop();
    
    /// This function closes the current scene
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
    /// This function destriy the entity
    /// - Parameter entity: entity handle
    void DestroyEntity(Entity entity);
    /// This function duplicate the entity
    /// - Parameter entity: entity to be duplicated
    [[nodiscard]] Entity DuplicateEntity(Entity entity);
    
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
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);
    /// This function set the selected Entity
    void SetSelectedEntity(entt::entity entity);
    /// This function clears the selected entity
    void ClearSelectedEntity();
    /// This function set the entity deletion callback
    /// - Parameter callback: callback funtion
    void SetEntityDestroyedCallback(const std::function<void(const Entity&)>& callback);
    
    /// This funcion sets the skybox asset handle
    /// - Parameter handle: skybox handle
    void SetSkyboxAsset(AssetHandle handle);
    /// This funcion set the IBL flag
    /// - Parameter value: value
    void SetIBLFlag(bool value);
    /// This funcion set the Irradiance flag
    /// - Parameter value: value
    void SetIrradianceFlag(bool value);

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;
    /// This function returns the reference of registry
    entt::registry& GetRegistry();
    /// This function returns the max ID given to entity
    uint32_t GetMaxEntityId() const;
    /// This function finds the main camera entity
    Entity GetMainCameraEntity();
    /// This function returns the selected entity handle
    const std::vector<entt::entity>& GetSelectedEntity() const;
    
    /// This function returns entity with id as specified, or empty entity if cannot be found - caller must check
    /// - Parameter id: UUID of entity
    Entity TryGetEntityWithUUID(UUID id) const;
    /// This function return entity with id as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter id: UUID for entity
    Entity GetEntityWithUUID(UUID id) const;
    /// This function return entity with entity handle as specified. entity is expected to exist (runtime error if it doesn't)
    /// - Parameter entityHandle: handle for entity
    Entity GetEntityWithEntityHandle(int32_t entityHandle) const;
    /// This function check entity is selected
    /// - Parameter entity: Selected Entity
    bool IsEntitySelected(entt::entity entity) const;
    
    /// This function returns the skybox asset
    AssetHandle GetSkyboxAsset() const;
    /// This function returns the IBL flag
    bool UseIBL() const;
    /// This function returns the Irradiance flag
    bool UseIrradiance() const;
    
    /// This function pass the reference of Direction light to control from client
    DirectionLight& GetDirectionLight();
    
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
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Scene);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function renders the scene insice pass
    /// - Parameters:
    ///   - renderer: Scene renderer instance
    ///   - isEditing: flag is editing
    void RenderScene(SceneRenderer& renderer, bool isEditing);
    
    // Component creation and destroy callbacks
    void OnRigidBodyComponentConstruct();
    void OnRigidBodyComponentDestroy();
    void OnBox3DColliderComponentConstruct();
    void OnBox3DColliderComponentDestroy();
    void OnSphereColliderComponentConstruct();
    void OnSphereColliderComponentDestroy();
    void OnCapsuleColliderComponentConstruct();
    void OnCapsuleColliderComponentDestroy();
    void OnJointComponentConstruct();
    void OnJointComponentDestroy();
    
    // Member Variables ---------------------------------------------------------------------------------------------
    // Scene Utils ----------------------
    std::string m_name {};
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};
    
    // EnTT -----------------------------
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;
    uint32_t m_numEntities = 0;
    int32_t m_maxEntityID = -1;
    
    // Entity ---------------------------
    EntityMap m_entityIDMap;
    std::vector<entt::entity> m_selectedEntities;
    std::function<void(const Entity&)> m_onEntityDestroyedCallback;
    
    // Environment ----------------------
    bool m_useIBL {true};
    bool m_useIrradiance {false};
    AssetHandle m_skyboxAsset;
    DirectionLight m_directionLight;
    
    friend class SceneSerializer;
    friend class Entity;
  };
} // namespace IKan
