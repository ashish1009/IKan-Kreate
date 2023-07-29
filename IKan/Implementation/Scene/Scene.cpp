//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Scene.hpp"
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
  static void CopyComponent(entt::registry& dstRegistry, entt::registry& srcRegistry, const std::unordered_map<UUID, entt::entity>& enttMap)
  {
    auto srcEntities = srcRegistry.view<T>();
    for (auto srcEntity : srcEntities)
    {
      entt::entity destEntity = enttMap.at(srcRegistry.get<IDComponent>(srcEntity).ID);
      
      auto& srcComponent = srcRegistry.get<T>(srcEntity);
      auto& destComponent = dstRegistry.emplace_or_replace<T>(destEntity, srcComponent);
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

  Ref<Scene> Scene::Create(uint32_t maxEntityCapacity)
  {
    return CreateRef<Scene>(maxEntityCapacity);
  }

  Scene::Scene(uint32_t maxEntityCapacity)
  : m_registryCapacity(maxEntityCapacity)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Creating Scene ...");
    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
  }

  Scene::~Scene()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Scene, "Destroying Scene!!!");
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
    
    auto childIds = entity.Children(); // need to take a copy of children here, because the collection is mutated below
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
  
  void Scene::SetEntityDestroyedCallback(const std::function<void(Entity)>& callback)
  {
    m_onEntityDestroyedCallback = callback;
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

} // namespace IKan
