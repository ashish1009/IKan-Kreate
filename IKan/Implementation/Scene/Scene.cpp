//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "Scene.hpp"

#include "Scene/Component.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene2D.hpp"
#include "Scene/Scene3D.hpp"

namespace IKan
{
  /// This function resize/reserve the registry capcity
  template<typename... Component>
  static void ReserveRegistry(ComponentGroup<Component...>, entt::registry& registry, int32_t capacity)
  {
    registry.reserve<Component...>(capacity);
  }

  template<typename T>
  static void CopyComponentIfExists(entt::entity dst, entt::entity src, entt::registry& registry)
  {
    if (registry.has<T>(src))
    {
      auto& srcComponent = registry.get<T>(src);
      registry.emplace_or_replace<T>(dst, srcComponent);
    }
  }
  
  template<typename T>
  static void CopyComponentIfExists(entt::entity dst, entt::registry& dstRegistry, entt::entity src, entt::registry& srcRegistry)
  {
    if (srcRegistry.has<T>(src))
    {
      auto& srcComponent = srcRegistry.get<T>(src);
      dstRegistry.emplace_or_replace<T>(dst, srcComponent);
    }
  }
  
  template<typename T>
  static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
  {
    auto srcEntities = srcRegistry.view<T>();
    for (auto srcEntity : srcEntities)
    {
      entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);
      
      auto& srcComponent = srcRegistry.get<T>(srcEntity);
      [[maybe_unused]] auto& destComponent = dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
    }
  }

  static std::string_view SceneTypeString(SceneType sceneType)
  {
    switch (sceneType)
    {
      case SceneType::_2D: return "2D";
      case SceneType::_3D: return "3D";
      case SceneType::None:
      default:
        IK_ASSERT(false, "Invalid Scene");
        break;
    }
    return "";
  }

  Ref<Scene> Scene::Create(SceneType sceneType, const std::string& name, uint32_t maxEntityCapacity)
  {
    switch (sceneType)
    {
      case SceneType::_2D: return CreateRef<Scene2D>(name, maxEntityCapacity);
      case SceneType::_3D: return CreateRef<Scene3D>(name, maxEntityCapacity);
      case SceneType::None:
      default:
        IK_ASSERT(false, "Invalid Scene");
        break;
    }
    return nullptr;
  }
  
  Scene::Scene(SceneType sceneType, const std::string& name, uint32_t maxEntityCapacity)
  : m_sceneType(sceneType), m_name(name), m_registryCapacity(maxEntityCapacity)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Creating {0} Scene", SceneTypeString(m_sceneType));
    IK_LOG_TRACE(LogModule::Scene, "  Name              : {0}", m_name);
    IK_LOG_TRACE(LogModule::Scene, "  Registry Capacity : {0}", m_registryCapacity);

    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
  }
  
  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Destroying {0} Scene. (Registry Capacity {1})", m_name, m_registryCapacity);
  }
  
  void Scene::OnUpdateEditor()
  {
    IK_PERFORMANCE("Scene::OnUpdateEditor");
  }
  void Scene::OnUpdateRuntime(TimeStep ts)
  {
    IK_PERFORMANCE("Scene::OnUpdateRuntime");
  }
  
  void Scene::OnRuntimeEventHandler(Event& event)
  {
    
  }
  void Scene::OnEditorEventHandler(Event& event)
  {

  }
  
  void Scene::OnRenderEditor(const EditorCamera &camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderEditor");
    
    // Render 3D Scene
    renderer.BeginScene({ camera, camera.GetPosition(), camera.GetViewMatrix(), camera.GetNearClip(), camera.GetFarClip(), camera.GetVerticalFOV()});
    RenderScene(renderer, true);
    renderer.EndScene();
  }
  void Scene::OnRenderRuntime([[maybe_unused]] TimeStep ts, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderRuntime");
    
    Entity cameraEntity = GetMainCameraEntity();
    if (!cameraEntity)
    {
      return;
    }
    
    const auto& mainCamera = cameraEntity.GetComponent<CameraComponent>().camera;
    const auto& cameraTransformComp = cameraEntity.GetComponent<TransformComponent>();
    
    // Render 3D Scene
    renderer.BeginScene({ mainCamera, cameraTransformComp.Position(), glm::inverse(cameraTransformComp.Transform()), 0.01f, 10000.0f, mainCamera.GetRadPerspectiveVerticalFOV() });
    RenderScene(renderer, false);
    renderer.EndScene();
  }
  void Scene::OnRenderSimulation([[maybe_unused]] TimeStep ts, const EditorCamera& camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderSimulation");
    
    // Render 3D Scene
    renderer.BeginScene({ camera, camera.GetPosition(), camera.GetViewMatrix(), camera.GetNearClip(), camera.GetFarClip(), camera.GetVerticalFOV()});
    RenderScene(renderer, false);
    renderer.EndScene();
  }
  void Scene::RenderScene(SceneRenderer& renderer, bool isEditing)
  {
    IK_PERFORMANCE("Scene::RenderScene");
    
    // Submit the Meshes --------------------------------------------------
    auto meshView = m_registry.view<TransformComponent, MeshComponent>();
    for (const auto& entityHandle : meshView)
    {
      Entity entity{entityHandle, this};
      const auto& transformComp = entity.GetComponent<TransformComponent>();
      const auto& meshComp = entity.GetComponent<MeshComponent>();
      const auto& visibilityComp = entity.GetComponent<VisibilityComponent>();
      if (meshComp.mesh != 0 and meshComp.enable and (visibilityComp.isVisible or (!visibilityComp.isVisible and !isEditing)))
      {
        // Render Selected Mesh ---------------------------------------------
        Entity entity = {entityHandle, this};
        
        renderer.SubmitMesh(meshComp.mesh, transformComp.Transform(), meshComp.materialTable, meshComp.tilingFactor);
      }
    } // For each Mesh Entity
  }
  
  void Scene::OnRuntimeStart()
  {
    IK_PROFILE();
  }
  void Scene::OnRuntimeStop()
  {
    IK_PROFILE();
  }
  void Scene::OnSimulationStart()
  {
    IK_PROFILE();
  }
  void Scene::OnSimulationStop()
  {
    IK_PROFILE();
  }
  
  void Scene::OnClose()
  {
    IK_PROFILE();
  }
  void Scene::CopyTo(Ref<Scene> &target)
  {
    IK_PROFILE();
    std::unordered_map<UUID, entt::entity> enttMap;
    auto idComponents = m_registry.view<IDComponent>();
    for (auto entity : idComponents)
    {
      auto uuid = m_registry.get<IDComponent>(entity).ID;
      auto name = m_registry.get<TagComponent>(entity).tag;
      Entity e = target->CreateEntityWithID(uuid, name);
      enttMap[uuid] = e.m_entityHandle;
    }
    
    CopyComponent<VisibilityComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<TagComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<RelationshipComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<TransformComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<CameraComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<MeshComponent>(target->m_registry, m_registry, enttMap);

    // Sort IdComponent by by entity handle (which is essentially the order in which they were created)
    // This ensures a consistent ordering when iterating IdComponent (for example: when rendering scene hierarchy panel)
    target->m_registry.sort<IDComponent>([&target](const auto lhs, const auto rhs)
                                         {
      auto lhsEntity = target->m_entityIDMap.find(lhs.ID);
      auto rhsEntity = target->m_entityIDMap.find(rhs.ID);
      return static_cast<uint32_t>(lhsEntity->second) < static_cast<uint32_t>(rhsEntity->second);
    });
    
    target->m_viewportWidth = m_viewportWidth;
    target->m_viewportHeight = m_viewportHeight;
    target->m_name = m_name;
  }
  
  void Scene::SetViewportSize(uint32_t width, uint32_t height)
  {
    IK_PROFILE();
    if (m_viewportWidth == width and m_viewportHeight == height)
    {
      return;
    }
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
  
  Entity Scene::CreateEntity(const std::string& name)
  {
    IK_PROFILE();
    return CreateChildEntity({}, name);
  }
  
  Entity Scene::CreateChildEntity(Entity parent, const std::string& name)
  {
    IK_PROFILE();
    Entity entity = CreateEntityWithID({}, name);
    
    // Debug Logs
    IK_LOG_TRACE(LogModule::Scene, "Stored Entity in Scene");
    IK_LOG_TRACE(LogModule::Scene, "  Name    {0}", entity.GetName());
    IK_LOG_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_LOG_TRACE(LogModule::Scene, "  ID      {0}", (uint32_t)entity.GetUUID());
    IK_LOG_TRACE(LogModule::Scene, "  Number of entities Added in Scene  {0}", m_numEntities);
    IK_LOG_TRACE(LogModule::Scene, "  Max ID given to entity             {0}", m_maxEntityID);

    if (parent)
    {
      IK_ASSERT(false, "Implement later");
    }

    return entity;
  }
  
  Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
  {
    IK_PROFILE();
    
    // TODO: Resize regirtry later
    IK_ASSERT(m_registry.size() < m_registryCapacity, "Entity registry reach max limit !!!")
    
    // If same UUID is already present in scene
    IK_ASSERT(m_entityIDMap.find(uuid) == m_entityIDMap.end(), "Same UUID is already present in Scene");
    
    auto entity = Entity { m_registry.create(), this };
    
    // Add Mendatory Components
    entity.AddComponent<IDComponent>(uuid);
    entity.AddComponent<VisibilityComponent>();
    entity.AddComponent<TransformComponent>(); // By default Unit transform
    entity.AddComponent<TagComponent>(name);
    entity.AddComponent<RelationshipComponent>(); // By default No parent and No child
    
    // Store in Entity ID Map
    m_entityIDMap[uuid] = entity;
    
    // Updating the Max entity ID
    m_maxEntityID = (int32_t)((uint32_t)entity);
    ++m_numEntities;
    
    return entity;
  }

  void Scene::DestroyEntity(Entity entity, bool firstCall)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Deleting Entity from Scene");
    IK_LOG_TRACE(LogModule::Scene, "  Name    {0}", entity.GetComponent<TagComponent>().tag.c_str());
    IK_LOG_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_LOG_TRACE(LogModule::Scene, "  ID      {0}", (uint32_t)entity.GetComponent<IDComponent>().ID);
    IK_LOG_TRACE(LogModule::Scene, "  Number of entities Added in Scene  {0}", m_numEntities);
    IK_LOG_TRACE(LogModule::Scene, "  Max ID given to entity             {0}", m_maxEntityID);
    
    if (m_onEntityDestroyedCallback)
    {
      m_onEntityDestroyedCallback(entity);
    }
    
    // Delete all children
    for (size_t i = 0; i < entity.Children().size(); i++)
    {
      auto childId = entity.Children()[i];
      Entity child = GetEntityWithUUID(childId);
      DestroyEntity(child, false);
    }
    
    // Delete This child from its parent if exist
    if (firstCall)
    {
      IK_ASSERT(false, "Implement later");
    }
    
    m_entityIDMap.erase(entity.GetUUID());
    m_registry.destroy(entity.m_entityHandle);
    
    --m_numEntities;
  }
  
  Entity Scene::DuplicateEntity(Entity entity)
  {
    IK_PROFILE();
    
    Entity newEntity;
    if (entity.HasComponent<TagComponent>())
    {
      newEntity = CreateEntity(entity.GetComponent<TagComponent>().tag);
    }
    else
    {
      newEntity = CreateEntity("Empty Entity");
    }
    
    CopyComponentIfExists<VisibilityComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<TransformComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<CameraComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<MeshComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);

    auto childIds = entity.Children();
    for (auto childId : childIds)
    {
      IK_ASSERT(false ,"Implement later");
    }
    IK_ASSERT(false ,"Implement later");
        
    return newEntity;
  }
  
  void Scene::SetName(const std::string &name)
  {
    m_name = name;
  }
  void Scene::SetSelectedEntity(entt::entity entity)
  {
    m_selectedEntities.push_back(entity);
  }
  
  void Scene::ClearSelectedEntity()
  {
    m_selectedEntities.clear();
  }

  void Scene::SetEntityDestroyedCallback(const std::function<void(const Entity&)>& callback)
  {
    m_onEntityDestroyedCallback = callback;
  }

  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
  
  Entity Scene::TryGetEntityWithUUID(UUID id) const
  {
    IK_PROFILE();
    if (const auto iter = m_entityIDMap.find(id); iter != m_entityIDMap.end())
    {
      return iter->second;
    }
    return Entity{};
  }
  Entity Scene::GetEntityWithUUID(UUID id) const
  {
    IK_ASSERT(m_entityIDMap.find(id) != m_entityIDMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
    return m_entityIDMap.at(id);
  }
  Entity Scene::GetEntityWithEntityHandle(int32_t entityHandle) const
  {
    auto& ID = m_registry.get<IDComponent>(static_cast<entt::entity>(entityHandle)).ID;
    return GetEntityWithUUID(ID);
  }
  
  Entity Scene::GetMainCameraEntity()
  {
    IK_PERFORMANCE("Scene::GetMainCameraEntity");
    auto view = m_registry.view<CameraComponent>();
    for (const auto& entity : view)
    {
      auto& comp = view.get<CameraComponent>(entity);
      if (comp.primary)
      {
        IK_ASSERT(comp.camera.GetOrthographicSize() or comp.camera.GetDegPerspectiveVerticalFOV(), "Camera is not fully initialized");
        return { entity, this };
      }
    }
    return {};
  }
  
  bool Scene::IsEntitySelected(entt::entity entity) const
  {
    for (const auto& selectedEntity : m_selectedEntities)
    {
      if (selectedEntity == entity)
      {
        return true;
      }
    }
    return false;
  }

} // namespace IKan
