//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Scene.hpp"
#include "Scene/CoreEntity.hpp"
#include "Project/Project.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/SceneRenderer.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Asset/AssetManager.hpp"

using namespace reactphysics3d;

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
    IK_LOG_TRACE(LogModule::Scene, "Creating Scene ...");
    IK_LOG_TRACE(LogModule::Scene, "  Name               {0}", m_name);
    IK_LOG_TRACE(LogModule::Scene, "  Registry Capacity  {0}", m_registryCapacity);
    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
    
    m_registry.on_construct<StaticMeshComponent>().connect<&Scene::OnStaticMeshComponentConstruct>(this);
    m_registry.on_destroy<StaticMeshComponent>().connect<&Scene::OnStaticMeshComponentDestroy>(this);
  }
  
  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Scene, "Destroying Scene!!!");
    IK_LOG_TRACE(LogModule::Scene, "  Name               {0}", m_name);
    IK_LOG_TRACE(LogModule::Scene, "  Registry Capacity  {0}", m_registryCapacity);
  }
  
  void Scene::OnUpdateEditor(TimeStep ts)
  {
    IK_PERFORMANCE_FUN();
  }
  
  void Scene::OnUpdateRuntime(TimeStep ts)
  {
    IK_PERFORMANCE_FUN();
    m_physicsScene->OnUpdate(ts);
  }
  
  void Scene::OnRenderEditor(const EditorCamera &camera, const Ref<SceneRenderer> renderer)
  {
    IK_PERFORMANCE_FUN();
    renderer->BeginScene({ camera.GetPosition(), camera.GetUnReversedViewProjection(), camera.GetDistance()});
    
    Render3DEntities(renderer);
    renderer->EndScene();

    Renderer2D::BeginBatch(camera.GetUnReversedViewProjection(), camera.GetViewMatrix());
    Render2DEntities();
    Renderer2D::EndBatch();
  }
  
  void Scene::OnRenderRuntime(TimeStep ts, const Ref<SceneRenderer> renderer)
  {
    Entity cameraEntity = GetMainCameraEntity();
    if (!cameraEntity)
    {
      return;
    }
    
    const auto& mainCamera = cameraEntity.GetComponent<CameraComponent>().camera;
    const auto& cameraTransformComp = cameraEntity.GetComponent<TransformComponent>();
    
    renderer->BeginScene({
      cameraTransformComp.Position(),
      mainCamera.GetUnReversedProjectionMatrix() * glm::inverse(cameraTransformComp.Transform()),
      1});
    Render3DEntities(renderer);
    renderer->EndScene();

    Renderer2D::BeginBatch(mainCamera.GetUnReversedProjectionMatrix() * glm::inverse(cameraTransformComp.Transform()),
                           glm::inverse(cameraTransformComp.Transform()));
    Render2DEntities();
    Renderer2D::EndBatch();
  }
  
  void Scene::OnRenderSimulation(TimeStep ts, const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer)
  {
    IK_PERFORMANCE_FUN();
    renderer->BeginScene({
      editorCamera.GetPosition(),
      editorCamera.GetUnReversedViewProjection(),
      editorCamera.GetDistance()});
    Render3DEntities(renderer);
    renderer->EndScene();

    Renderer2D::BeginBatch(editorCamera.GetUnReversedViewProjection(), editorCamera.GetViewMatrix());
    Render2DEntities();
    Renderer2D::EndBatch();
  }

  void Scene::Render2DEntities()
  {
    // Render All Quad Entities
    {
      auto view = m_registry.view<TransformComponent, QuadComponent>();
      for (const auto& entity : view)
      {
        const auto& [transformComp, quadComp] = view.get<TransformComponent, QuadComponent>(entity);
        if (quadComp.texture == 0)
        {
          Renderer2D::DrawQuad(transformComp.Transform(), quadComp.color, (int32_t)entity);
        }
        else
        {
          Ref<Image> texture = AssetManager::GetAsset<Image>(quadComp.texture);
          Renderer2D::DrawQuad(transformComp.Transform(), texture, quadComp.color, quadComp.tilingFactor, (int32_t)entity);
        }
      } // For each Quad Entity
    }
    
    // Render All Circle Entities
    {
      auto view = m_registry.view<TransformComponent, CircleComponent>();
      for (const auto& entity : view)
      {
        const auto& [transformComp, circleComp] = view.get<TransformComponent, CircleComponent>(entity);
        if (circleComp.texture == 0)
        {
          Renderer2D::DrawCircle(transformComp.Transform(), circleComp.color, circleComp.thickness, circleComp.fade, (int32_t)entity);
        }
        else
        {
          Ref<Image> texture = AssetManager::GetAsset<Image>(circleComp.texture);
          Renderer2D::DrawCircle(transformComp.Transform(), texture, circleComp.color, 1.0f, circleComp.thickness, circleComp.fade, (int32_t)entity);
        }
      } // For each Quad Entity
    }
    
    // Render All Text Entities
    {
      auto view = m_registry.view<TransformComponent, TextComponent>();
      for (const auto& entity : view)
      {
        const auto& [transformComp, textComp] = view.get<TransformComponent, TextComponent>(entity);
        if (textComp.assetHandle == 0)
        {
          Renderer2D::RenderText(textComp.textString, Font::GetDefaultFont(), transformComp.Position(), transformComp.Scale(), textComp.color, (int32_t)entity);
        }
        else
        {
          Ref<Font> font = AssetManager::GetAsset<Font>(textComp.assetHandle);
          if (font == nullptr)
          {
            font = Font::GetDefaultFont();
          }
          Renderer2D::RenderText(textComp.textString, font, transformComp.Position(), transformComp.Scale(), textComp.color, (int32_t)entity);
        }
      } // For each Quad Entity
    }
  }
  
  void Scene::Render3DEntities(Ref<SceneRenderer> renderer)
  {
    // Submit the Lights
    auto lightView = m_registry.view<TransformComponent, PointLightComponent>();
    for (const auto& entityHandle : lightView)
    {
      const auto& [transformComp, pointLight] = lightView.get<TransformComponent, PointLightComponent>(entityHandle);
      if (pointLight.active)
      {
        renderer->SubmitPointLight({(float)pointLight.active, transformComp.Position(), pointLight.radiance});
      }
    } // For each MEsh Entity

    // Submit the Meshes
    auto meshView = m_registry.view<TransformComponent, StaticMeshComponent>();
    for (const auto& entityHandle : meshView)
    {
      const auto& [transformComp, staticMeshComp] = meshView.get<TransformComponent, StaticMeshComponent>(entityHandle);
      if (staticMeshComp.staticMesh != 0)
      {
        if (IsEntitySelected(entityHandle))
        {
          // TODO: Do it only for Editor
          renderer->SubmitSelectedMeshSource(AssetManager::GetAsset<MeshSource>(staticMeshComp.staticMesh), transformComp.Transform());
        }
        else
        {
          renderer->SubmitMeshSource(AssetManager::GetAsset<MeshSource>(staticMeshComp.staticMesh), transformComp.Transform());
        }
      }
    } // For each Mesh Entity
  }
    
  void Scene::OnRuntimeStart()
  {
    IK_PROFILE();
    InitializePhysicsWorld();
  }
  
  void Scene::OnRuntimeStop()
  {
    IK_PROFILE();
    DestroyPhysicsWorld();
  }
  void Scene::OnSimulationStart()
  {
    IK_PROFILE();
    InitializePhysicsWorld();
  }
  void Scene::OnSimulationStop()
  {
    IK_PROFILE();
    DestroyPhysicsWorld();
  }
  
  void Scene::OnClose()
  {
    while (m_unsavedAssetHandles.size())
    {
      const AssetMetadata& metadata = m_unsavedAssetHandles.top();
      
      // Hack to avoid mesh Deletion
      if (metadata.type != AssetType::MeshSource)
      {
        bool deleted = Utils::FileSystem::DeleteFile(Project::GetActive()->GetAssetDirectory() / metadata.filePath);
        if (!deleted)
        {
          IK_LOG_ERROR(LogModule::Scene, "Failed to delete folder {0}", metadata.filePath.string().c_str());
          return;
        }
      }
      AssetManager::OnAssetDeleted(metadata.handle);
      m_unsavedAssetHandles.pop();
    }
  }
  
  void Scene::DestroyPhysicsWorld()
  {
    IK_PROFILE();
    m_physicsScene.reset();
  }
  
  void Scene::InitializePhysicsWorld()
  {
    IK_PROFILE();
    m_physicsScene = CreateRef<PhysicsScene>(m_physicsSettings, this);
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
    CopyComponent<TransformComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<CameraComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<QuadComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<CircleComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<TextComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<StaticMeshComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<RigidBodyComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<Box3DColliderComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<SphereColliderComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<CapsuleColliderComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<JointComponent>(target->m_registry, m_registry, enttMap);
    CopyComponent<PointLightComponent>(target->m_registry, m_registry, enttMap);

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
  }
  
  void Scene::SetViewportSize(uint32_t width, uint32_t height)
  {
    if (m_viewportWidth == width and m_viewportHeight == height)
    {
      return;
    }

    m_viewportWidth = width;
    m_viewportHeight = height;

    UpdateCamerasViewport();
  }
  
  Entity Scene::CreateEntity(const std::string& name)
  {
    return CreateChildEntity({}, name);
  }
  
  Entity Scene::CreateChildEntity(Entity parent, const std::string& name)
  {
    Entity entity = CreateEntityWithID({}, name);
    
    // Add Parent if requred
    if (parent)
    {
      entity.SetParent(parent);
    }
    
    // Debug Logs
    IK_LOG_TRACE(LogModule::Scene, "Stored Entity in Scene");
    IK_LOG_TRACE(LogModule::Scene, "  Name    {0}", entity.GetComponent<TagComponent>().tag.c_str());
    IK_LOG_TRACE(LogModule::Scene, "  Handle  {0}", (uint32_t)entity);
    IK_LOG_TRACE(LogModule::Scene, "  ID      {0}", (uint32_t)entity.GetComponent<IDComponent>().ID);
    IK_LOG_TRACE(LogModule::Scene, "  Number of entities Added in Scene  {0}", m_numEntities);
    IK_LOG_TRACE(LogModule::Scene, "  Max ID given to entity             {0}", m_maxEntityID);
    
    return entity;
  }
  
  Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
  {
    // If max capcity reached then resize the capacity
    if (m_registry.size() >= m_registryCapacity)
    {
      IK_ASSERT(false, "Temp Assert to check performance")
      m_registryCapacity *= 2;
      ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
    }
    
    // Create new entity
    auto entity = Entity { m_registry.create(), this };
    
    // Add Mendatory Components
    [[maybe_unused]] auto& idComponent = entity.AddComponent<IDComponent>(uuid);
    
    entity.AddComponent<TransformComponent>();
    if (!name.empty())
    {
      entity.AddComponent<TagComponent>(name);
    }
    
    entity.AddComponent<RelationshipComponent>();
    
    // Store in Entity ID Map
    IK_ASSERT(m_entityIDMap.find(uuid) == m_entityIDMap.end());
    m_entityIDMap[uuid] = entity;
    
    // Updating the Max entity ID
    m_maxEntityID = (int32_t)((uint32_t)entity);
    
    ++m_numEntities;
    
    return entity;
  }
  
  void Scene::DestroyEntity(Entity entity)
  {
    if (m_onEntityDestroyedCallback)
    {
      m_onEntityDestroyedCallback(entity);
    }
    
    for (size_t i = 0; i < entity.Children().size(); i++)
    {
      auto childId = entity.Children()[i];
      Entity child = GetEntityWithUUID(childId);
      DestroyEntity(child);
    }
    
    m_entityIDMap.erase(entity.GetUUID());
    m_registry.destroy(entity.m_entityHandle);
    
    --m_numEntities;
  }

  Entity Scene::DuplicateEntity(Entity entity)
  {
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
      newEntity = CreateEntity();
    }
    
    CopyComponentIfExists<TransformComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<CameraComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<SpriteRendererComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<QuadComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<CircleComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<TextComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<StaticMeshComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<RigidBodyComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<Box3DColliderComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<SphereColliderComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<CapsuleColliderComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<JointComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<PointLightComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);

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
    // If new parent is already child of 'entity'
    if (parent.IsDescendantOf(entity))
    {
      // Unparent the 'parent' first
      UnparentEntity(parent, false);
      
      // If Current 'entity' already have some parent
      Entity newParent = TryGetEntityWithUUID(entity.GetParentUUID());
      if (newParent)
      {
        // Unperent current entity
        UnparentEntity(entity, false);
        
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
        UnparentEntity(entity, false);
      }
    }
    
    // Update parent UUID of 'entity'
    entity.SetParentUUID(parent.GetUUID());
    // Update children of 'parent'
    parent.Children().push_back(entity.GetUUID());
    
#if 0
    // Update local space of 'entity'
    ConvertToLocalSpace(entity);
#endif
  }
  
  void Scene::UnparentEntity(Entity entity, bool convertToWorldSpace)
  {
    // Get the previous parent of 'entity'
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    if (!parent)
    {
      return;
    }

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
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    
    if (!parent)
    {
      return;
    }
    auto& transform = entity.GetTransform();
    glm::mat4 parentTransform = GetWorldSpaceTransformMatrix(parent);
    glm::mat4 localTransform = glm::inverse(parentTransform) * transform.Transform();
    glm::vec3 position, scale, rotation;
    Utils::Math::DecomposeTransform(localTransform, position, rotation, scale);
    transform.UpdatePosition(position);
    transform.UpdateScale(scale);
    transform.UpdateRotation(rotation);
  }
  
  void Scene::ConvertToWorldSpace(Entity entity)
  {
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    
    if (!parent)
    {
      return;
    }
    
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
    glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);
    TransformComponent transformComponent = entity.GetComponent<TransformComponent>();
    
    glm::vec3 position, scale, rotation;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    transformComponent.UpdatePosition(position);
    transformComponent.UpdateScale(scale);
    transformComponent.UpdateRotation(rotation);
    
    return transformComponent;
  }

  void Scene::UpdateCamerasViewport()
  {
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& comp = view.get<CameraComponent>(entity);
      comp.camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
    }
  }
  
  void Scene::SetName(const std::string &name)
  {
    m_name = name;
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

  void Scene::SetSelectedEntity(entt::entity entity)
  {
    m_selectedEntities.push_back(entity);
  }
  
  void Scene::ClearSelectedEntity()
  {
    m_selectedEntities.clear();
  }
  
  void Scene::SetEntityDestroyedCallback(const std::function<void(Entity)>& callback)
  {
    m_onEntityDestroyedCallback = callback;
  }
  
  void Scene::AddUnsavedAssetHandles(const AssetMetadata& metadata)
  {
    m_unsavedAssetHandles.push(metadata);
  }
  
  void Scene::ClearUnsavedAssets()
  {
    while (m_unsavedAssetHandles.size())
    {
      m_unsavedAssetHandles.pop();
    }
  }
  
  Entity Scene::GetMainCameraEntity()
  {
    auto view = m_registry.view<CameraComponent>();
    for (auto entity : view)
    {
      auto& comp = view.get<CameraComponent>(entity);
      if (comp.primary)
      {
        IK_ASSERT(comp.camera.GetOrthographicSize() or comp.camera.GetDegPerspectiveVerticalFOV(),
                  "Camera is not fully initialized");
        return { entity, this };
      }
    }
    return {};
  }
  
  void Scene::OnStaticMeshComponentConstruct(entt::registry& registry, entt::entity handle)
  {
  }
  
  void Scene::OnStaticMeshComponentDestroy(entt::registry& registry, entt::entity entityHandle)
  {
    Entity entity(entityHandle, this);
    const auto& assetHandle = entity.GetComponent<StaticMeshComponent>().staticMesh;
    m_unsavedAssetHandles.push(AssetManager::GetMetadata(assetHandle));
  }
  
  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
  const std::vector<entt::entity>& Scene::GetSelectedEntity() const
  {
    return m_selectedEntities;
  }

  Entity Scene::GetEntityWithUUID(UUID id) const
  {
    IK_LOG_VERIFY(m_entityIDMap.find(id) != m_entityIDMap.end(), "Invalid entity ID or entity doesn't exist in scene!");
    return m_entityIDMap.at(id);
  }
  
  Entity Scene::GetEntityWithEntityHandle(int32_t entityHandle) const
  {
    auto& ID = m_registry.get<IDComponent>(static_cast<entt::entity>(entityHandle)).ID;
    return GetEntityWithUUID(ID);
  }

  Entity Scene::TryGetEntityWithUUID(UUID id) const
  {
    if (const auto iter = m_entityIDMap.find(id); iter != m_entityIDMap.end())
    {
      return iter->second;
    }
    return Entity{};
  }
  uint32_t Scene::GetMaxEntityId() const
  {
    return m_maxEntityID;
  }
  
  PhysicsSettings& Scene::GetPhysicsSettings()
  {
    return m_physicsSettings;
  }
  
  reactphysics3d::DebugRenderer Scene::GetPhysicsDebugRenderer() const
  {
    return m_physicsScene->GetDebugRenderer();
  }

} // namespace IKan
