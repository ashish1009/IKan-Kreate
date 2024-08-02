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
} // namespace IKan
