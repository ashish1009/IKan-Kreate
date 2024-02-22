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

namespace IKan
{
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

    // Setters -----------------------------------------------------------------------------------------------------
    /// This function sets the scene name
    /// - Parameter name: scene name
    void SetName(const std::string& name);

    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the scene name
    const std::string& GetName() const;

    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(const std::string& name = "UntitledScene", uint32_t maxEntityCapacity = 200000);

    ASSET_TYPE(Scene);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Scene);

  private:
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

    friend class SceneSerializer;
    friend class Entity;
  };
} // namespace IKan
