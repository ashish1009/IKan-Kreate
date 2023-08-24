//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Scene.hpp"
#include "Scene/Entity.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/SceneRenderer.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  /// This function resize/reserve the registry capcity
  template<typename... Component>
  static void ReserveRegistry(ComponentGroup<Component...>, entt::registry& registry, int32_t capacity)
  {
    registry.reserve<Component...>(capacity);
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
  
  Ref<Scene> Scene::Create(const std::string& name, uint32_t maxEntityCapacity)
  {
    return CreateRef<Scene>(name, maxEntityCapacity);
  }
  
  Scene::Scene(const std::string& name, uint32_t maxEntityCapacity)
  : m_name(name), m_registryCapacity(maxEntityCapacity)
  {
    IK_LOG_TRACE(LogModule::Scene, "Creating Scene ...");
    IK_LOG_TRACE(LogModule::Scene, "  Name               {0}", m_name);
    IK_LOG_TRACE(LogModule::Scene, "  Registry Capacity  {0}", m_registryCapacity);
    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
  }
  
  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Scene, "Destroying Scene!!!");
  }
  
  void Scene::OnUpdateEditor(TimeStep ts)
  {
    
  }
  
  void Scene::OnUpdateRuntime(TimeStep ts)
  {
    // Update the Dynamics world with a constant time step
    m_physics3DWorld->update(ts);
    
    auto rigidBodyView = m_registry.view<RigidBodyComponent>();
    for (auto entityHandle : rigidBodyView)
    {
      Entity entity = { entityHandle, this };
      auto& rbc = entity.GetComponent<RigidBodyComponent>();
      auto& tc = entity.GetComponent<TransformComponent>();

      // Initial position and orientation of the rigid body
      reactphysics3d::Vector3 position (tc.Position().x, tc.Position().y, tc.Position().z);
      auto quaternion = glm::quat(tc.Rotation());
      reactphysics3d::Quaternion orientation(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
      
      // Create a rigid body in the world
      reactphysics3d::Transform transform(position, orientation);
      reactphysics3d::RigidBody* body = m_physics3DWorld->createRigidBody(transform);
      rbc.runtimeBody = body;
      
      // Change the type of the body to kinematic
      body->setType(RigidBodyComponent::ReactPhysicsBodyType(rbc.bodyType));
    }
  }
  
  void Scene::OnRenderEditor(const EditorCamera &editorCamera, const Ref<SceneRenderer> renderer)
  {
    renderer->BeginScene(editorCamera.GetUnReversedViewProjection());
    Render2DEntities();
    Render3DEntities(renderer);
    renderer->EndScene();
  }

  void Scene::OnRenderRuntime(TimeStep ts, const Ref<SceneRenderer> renderer)
  {
    Entity cameraEntity = GetMainCameraEntity();
    if (!cameraEntity)
    {
      return;
    }
    
    const auto& mainCamera = cameraEntity.GetComponent<CameraComponent>().camera;
    const auto& cameraTransform = cameraEntity.GetComponent<TransformComponent>().Transform();
    
    renderer->BeginScene(mainCamera.GetProjectionMatrix() * glm::inverse(cameraTransform));
    Render2DEntities();
    renderer->EndScene();
  }
  
  void Scene::OnRenderSimulation(TimeStep ts, const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer)
  {
    renderer->BeginScene(editorCamera.GetUnReversedViewProjection());
    Render2DEntities();
    renderer->EndScene();
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
    auto view = m_registry.view<TransformComponent, StaticMeshComponent>();
    for (const auto& entity : view)
    {
      const auto& [transformComp, staticMeshComp] = view.get<TransformComponent, StaticMeshComponent>(entity);
      if (staticMeshComp.staticMesh != 0)
      {
        renderer->SubmitMeshSource(AssetManager::GetAsset<MeshSource>(staticMeshComp.staticMesh), transformComp.Transform());
      }
    } // For each Quad Entity
  }
  
  void Scene::OnRuntimeStart()
  {
    // Create the world settings
    reactphysics3d::PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = 15;
    settings.defaultPositionSolverNbIterations = 5;
    settings.isSleepingEnabled = true;
    settings.gravity = reactphysics3d::Vector3 (0 , -9.81 , 0) ;

    // Create the physics world with your settings
    m_physics3DWorld = m_physics3DCommon.createPhysicsWorld(settings);
  }
  void Scene::OnRuntimeStop()
  {
    m_physics3DCommon.destroyPhysicsWorld(m_physics3DWorld);
  }
  void Scene::OnSimulationStart()
  {
    
  }
  void Scene::OnSimulationStop()
  {
    
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
  
  void Scene::DestroyEntity(Entity entity, bool excludeChildren, bool first)
  {
    if (m_onEntityDestroyedCallback)
    {
      m_onEntityDestroyedCallback(entity);
    }
    
    if (!excludeChildren)
    {
      // NOTE: don't make this a foreach loop because entt will move the children
      //       vector in memory as entities/components get deleted
      for (size_t i = 0; i < entity.Children().size(); i++)
      {
        auto childId = entity.Children()[i];
        Entity child = GetEntityWithUUID(childId);
        DestroyEntity(child, excludeChildren, false);
      }
    }
    
    if (first)
    {
      if (auto parent = entity.GetParent(); parent)
      {
        parent.RemoveChild(entity);
      }
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
    CopyComponentIfExists<SpriteRendererComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<QuadComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<CircleComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<TextComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<StaticMeshComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);
    CopyComponentIfExists<RigidBodyComponent>(newEntity.m_entityHandle, entity.m_entityHandle, m_registry);

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
  
  Entity Scene::TryGetEntityWithUUID(UUID id) const
  {
    if (const auto iter = m_entityIDMap.find(id); iter != m_entityIDMap.end())
    {
      return iter->second;
    }
    return Entity{};
  }

  void Scene::ParentEntity(Entity entity, Entity parent)
  {
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
    // Get the previous parent of 'entity'
    Entity parent = TryGetEntityWithUUID(entity.GetParentUUID());
    if (!parent)
    {
      return;
    }
    
    // Get the children of 'parent'
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
    auto& transform = entity.Transform();
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
    auto& entityTransform = entity.Transform();
    
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
    return transform * entity.Transform().Transform();
  }
  
  // TODO: Definitely cache this at some point
  TransformComponent Scene::GetWorldSpaceTransform(Entity entity)
  {
    glm::mat4 transform = GetWorldSpaceTransformMatrix(entity);
    TransformComponent transformComponent;
    
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

  void Scene::SetSelectedEntity(entt::entity entity)
  {
    m_selectedEntity = entity;
  }

  void Scene::SetEntityDestroyedCallback(const std::function<void(Entity)>& callback)
  {
    m_onEntityDestroyedCallback = callback;
  }
  void Scene::SetName(const std::string &name)
  {
    m_name = name;
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
  uint32_t Scene::GetMaxEntityId() const
  {
    return m_maxEntityID;
  }
  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  
  entt::entity Scene::GetSelectedEntity() const
  {
    return m_selectedEntity;
  }
  
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
} // namespace IKan
