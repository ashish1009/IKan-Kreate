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
  
  void Scene::OnRenderEditor(const EditorCamera &camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderEditor");
  }
  void Scene::OnRenderRuntime(TimeStep ts, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderRuntime");
  }
  
  void Scene::OnRenderSimulation(TimeStep ts, const EditorCamera& camera, SceneRenderer& renderer)
  {
    IK_PERFORMANCE("Scene::OnRenderSimulation");
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

  void Scene::SetName(const std::string &name)
  {
    m_name = name;
  }

  const std::string& Scene::GetName() const
  {
    return m_name;
  }

} // namespace IKan {
