//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "Scene.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"

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
  
  Ref<Scene> Scene::Create(const std::string& name, uint32_t maxEntityCapacity)
  {
    return CreateRef<Scene>(name, maxEntityCapacity);
  }
  
  Scene::Scene(const std::string& name, uint32_t maxEntityCapacity)
  : m_name(name), m_registryCapacity(maxEntityCapacity)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Creating {0} Scene. (Registry Capacity {1})", m_name, m_registryCapacity);
    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
    
    // Create physics scene
    m_physicsScene = CreateRef<PhysicsScene>(m_physicsSettings, this);
  }
  
  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Destroying {0} Scene. (Registry Capacity {1})", m_name, m_registryCapacity);
    m_physicsScene.reset();
  }
  
  void Scene::OnUpdateEditor()
  {
    IK_PERFORMANCE("Scene::OnUpdateEditor");
  }
  
  void Scene::OnUpdateRuntime(TimeStep ts)
  {
    IK_PERFORMANCE("Scene::OnUpdateRuntime");
    m_physicsScene->OnUpdate(ts);
  }
  
  void Scene::OnRuntimeEventHandler(Event& event)
  {
  }
  
  void Scene::OnRenderEditor(const EditorCamera &camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderEditor");
    
    // Render 3D Scene
    renderer.BeginScene({ camera, camera.GetPosition(), camera.GetViewMatrix(), camera.GetNearClip(), camera.GetFarClip(), camera.GetVerticalFOV()}, m_directionLight);
    RenderScene(renderer, true);
    renderer.EndScene();
  }
  void Scene::OnRenderRuntime(TimeStep ts, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderRuntime");
    IK_ASSERT(false);
  }
  
  void Scene::OnRenderSimulation(TimeStep ts, const EditorCamera& camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderSimulation");
    IK_ASSERT(false);
  }
  void Scene::RenderScene(SceneRenderer& renderer, bool isEditing)
  {
    IK_PERFORMANCE("Scene::RenderScene");
    // Submit the Meshes --------------------------------------------------
    auto meshView = m_registry.view<TransformComponent, MeshComponent>();
    for (const auto& entityHandle : meshView)
    {
      const auto& [transformComp, meshComp] = meshView.get<TransformComponent, MeshComponent>(entityHandle);
      if (meshComp.mesh != 0)
      {
        // Render Selected Mesh ---------------------------------------------
        Entity entity = {entityHandle, this};
        
        if (IsEntitySelected(entity) and isEditing)
        {
          renderer.SubmitSelectedMesh(meshComp.mesh, transformComp.Transform(), meshComp.materialTable, meshComp.tilingFactor);
        }
        else
        {
          renderer.SubmitMesh(meshComp.mesh, transformComp.Transform(), meshComp.materialTable, meshComp.tilingFactor);
        }
      }
    } // For each Mesh Entity
  }
  
  void Scene::OnRuntimeStart()
  {
    IK_PROFILE();
    IK_ASSERT(false);
  }
  void Scene::OnRuntimeStop()
  {
    IK_PROFILE();
    IK_ASSERT(false);
  }
  void Scene::OnSimulationStart()
  {
    IK_PROFILE();
    IK_ASSERT(false);
  }
  void Scene::OnSimulationStop()
  {
    IK_PROFILE();
    IK_ASSERT(false);
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
    
    CopyComponent<TagComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<RelationshipComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<TransformComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<MeshComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<RigidBodyComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<Box3DColliderComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<SphereColliderComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<JointComponent>(target->m_registry, m_registry, enttMap);

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
    target->m_physicsSettings = m_physicsSettings;
    
    if (m_physicsScene)
    {
      m_physicsScene.reset();
    }
    m_physicsScene = CreateRef<PhysicsScene>(m_physicsSettings, this);
  }
  
  void Scene::SetViewportSize(uint32_t width, uint32_t height)
  {
    RETURN_IF(m_viewportWidth == width and m_viewportHeight == height)
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
    IK_LOG_TRACE(LogModule::Scene, "  Name    {0}", entity.GetComponent<TagComponent>().tag.c_str());
    IK_LOG_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_LOG_TRACE(LogModule::Scene, "  ID      {0}", (uint32_t)entity.GetComponent<IDComponent>().ID);
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
  
  void Scene::DestroyEntity(Entity entity)
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
      DestroyEntity(child);
    }
    
    // Delete This child from its parent if exist
    auto parent = entity.GetParent();
    if (parent)
    {
      parent.RemoveChild(entity);
    }
    
    m_entityIDMap.erase(entity.GetUUID());
    m_registry.destroy(entity.m_entityHandle);
    
    --m_numEntities;
  }
  
  Entity Scene::DuplicateEntity(Entity entity)
  {
    IK_PROFILE();
    auto parentNewEntity = [&entity, scene = this](Entity newEntity)
    {
      if (auto parent = entity.GetParent(); parent)
      {
        newEntity.SetParentUUID(parent.GetUUID());
        parent.Children().push_back(newEntity.GetUUID());
      }
    };
    
    Entity newEntity;
    if (entity.HasComponent<TagComponent>())
    {
      newEntity = CreateEntity(entity.GetComponent<TagComponent>().tag);
    }
    else
    {
      newEntity = CreateEntity("Empty Entity");
    }
    
    CopyComponentIfExists<TransformComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<MeshComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    
    auto childIds = entity.Children();
    for (auto childId : childIds)
    {
      Entity childDuplicate = DuplicateEntity(GetEntityWithUUID(childId));
      
      // At this point childDuplicate is a child of entity, we need to remove it from that entity
      UnparentEntity(childDuplicate, false);
      
      childDuplicate.SetParentUUID(newEntity.GetUUID());
      newEntity.Children().push_back(childDuplicate.GetUUID());
    }
    
    parentNewEntity(newEntity);
    
    return newEntity;
  }
  
  void Scene::ParentEntity(Entity entity, Entity parent)
  {
    IK_PROFILE();
    // If new parent is already child of 'entity'
    if (parent.IsDescendantOf(entity))
    {
      // Unparent the 'parent' first
      UnparentEntity(parent);
      
      // If Current 'entity' already have some parent
      Entity newParent = TryGetEntityWithUUID(entity.GetParentUUID());
      if (newParent)
      {
        // Unperent current entity
        UnparentEntity(entity);
        
        // Set the 'newParent' as parent of 'parent' which was child of 'entity'
        ParentEntity(parent, newParent);
      }
    }
    else
    {
      // Get the previous parent of 'entity'
      Entity previousParent = TryGetEntityWithUUID(entity.GetParentUUID());
      
      // If Current 'entity' already have some parent
      if (previousParent)
      {
        // Unperent current entity
        UnparentEntity(entity);
      }
    }
    
    // Update parent UUID of 'entity'
    entity.SetParentUUID(parent.GetUUID());
    // Update children of 'parent'
    parent.Children().push_back(entity.GetUUID());
    
    // Update local space of 'entity'
    ConvertToLocalSpace(entity);
  }
  
  void Scene::UnparentEntity(Entity entity, bool convertToWorldSpace)
  {
    IK_PROFILE();
    // Get the previous parent of 'entity'
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    RETURN_IF (!parent)
    
    // Remove the entity from 'parent's' children
    auto& parentChildren = parent.Children();
    parentChildren.erase(std::remove(parentChildren.begin(), parentChildren.end(), entity.GetUUID()), parentChildren.end());
    
    if (convertToWorldSpace)
    {
      ConvertToWorldSpace(entity);
    }
    
    entity.SetParentUUID(0);
  }
  
  void Scene::ConvertToLocalSpace(Entity entity)
  {
    IK_PROFILE();
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    
    RETURN_IF (!parent)
    
    auto& transform = entity.GetTransform();
    glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);
    glm::mat4 localTransform = parentTransform * transform.Transform();
    glm::vec3 position, scale, rotation;
    Utils::Math::DecomposeTransform(localTransform, position, rotation, scale);
    transform.UpdatePosition(position);
    transform.UpdateScale(scale);
    transform.UpdateRotation(rotation);
  }
  
  void Scene::ConvertToWorldSpace(Entity entity)
  {
    IK_PROFILE();
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    
    RETURN_IF (!parent)
    
    glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);
    auto& entityTransform = entity.GetTransform();
    
    glm::vec3 position, scale, rotation;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    entityTransform.UpdatePosition(position);
    entityTransform.UpdateScale(scale);
    entityTransform.UpdateRotation(rotation);
  }
  
  glm::mat4 Scene::GetWorldSpaceTransformMatrix(Entity entity)
  {
    IK_PROFILE();
    glm::mat4 transform(1.0f);
    
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    if (parent)
    {
      transform = GetWorldSpaceTransformMatrix(parent);
    }
    return transform * entity.GetTransform().Transform();
  }
  
  // TODO: Definitely cache this at some point
  TransformComponent Scene::GetWorldSpaceTransform(Entity entity)
  {
    IK_PROFILE();
    glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);
    TransformComponent transformComponent = entity.GetTransform();
    
    glm::vec3 position, scale, rotation;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    transformComponent.UpdatePosition(position);
    transformComponent.UpdateScale(scale);
    transformComponent.UpdateRotation(rotation);
    
    return transformComponent;
  }
  
  const reactphysics3d::DebugRenderer& Scene::GetPhysicsDebugRenderer() const
  {
    return m_physicsScene->GetDebugRenderer();
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
  const std::vector<entt::entity>& Scene::GetSelectedEntity() const
  {
    return m_selectedEntities;
  }
  
  void Scene::SetEntityDestroyedCallback(const std::function<void(const Entity&)>& callback)
  {
    m_onEntityDestroyedCallback = callback;
  }
  
  void Scene::SetSkyboxAsset(AssetHandle handle)
  {
    m_skyboxAsset = handle;
    SceneRenderer::SubmitSkyboxImage(m_skyboxAsset);
  }
  
  void Scene::SetIBLFlag(bool value)
  {
    m_useIBL = value;
    SceneRenderer::SetIBLFlag(m_useIBL);
  }
  void Scene::SetIrradianceFlag(bool value)
  {
    m_useIrradiance = value;
    SceneRenderer::SetIrradianceFlag(m_useIrradiance);
  }
  
  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
  uint32_t Scene::GetMaxEntityId() const
  {
    return m_maxEntityID;
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
  
  Entity Scene::TryGetEntityWithUUID(UUID id) const
  {
    if (const auto iter = m_entityIDMap.find(id); iter != m_entityIDMap.end())
    {
      return iter->second;
    }
    return Entity{};
  }
  
  AssetHandle Scene::GetSkyboxAsset() const
  {
    return m_skyboxAsset;
  }
  
  bool Scene::UseIBL() const
  {
    return m_useIBL;
  }
  bool Scene::UseIrradiance() const
  {
    return m_useIrradiance;
  }
  
  DirectionLight& Scene::GetDirectionLight()
  {
    return m_directionLight;
  }
  
} // namespace IKan
