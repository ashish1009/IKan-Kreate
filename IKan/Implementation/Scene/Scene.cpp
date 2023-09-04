//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Scene.hpp"
#include "Scene/Component.hpp"
#include "Scene/Entity.hpp"

namespace IKan
{
  /// This function resize/reserve the registry capcity
  template<typename... Component>
  static void ReserveRegistry(ComponentGroup<Component...>, entt::registry& registry, int32_t capacity)
  {
    registry.reserve<Component...>(capacity);
  }

  Ref<Scene> Scene::Create(const std::string& name, uint32_t maxEntityCapacity)
  {
    return CreateRef<Scene>(name, maxEntityCapacity);
  }
  
  Scene::Scene(const std::string& name, uint32_t maxEntityCapacity)
  : m_name(name)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Scene, "Creating Scene ...");
    IK_LOG_TRACE(LogModule::Scene, "  Name               {0}", m_name);
    IK_LOG_TRACE(LogModule::Scene, "  Registry Capacity  {0}", m_registryCapacity);
    ReserveRegistry(AllComponents{}, m_registry, m_registryCapacity);
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
    
  }
  
  void Scene::OnUpdateRuntime(TimeStep ts)
  {
    
  }
  
  void Scene::OnRenderEditor(const EditorCamera &editorCamera, const Ref<SceneRenderer> renderer)
  {
    
  }
  
  void Scene::OnRenderRuntime(TimeStep ts, const Ref<SceneRenderer> renderer)
  {
    
  }
  
  void Scene::OnRenderSimulation(TimeStep ts, const EditorCamera& editorCamera, const Ref<SceneRenderer> renderer)
  {
    
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

  void Scene::CopyTo(Ref<Scene> &target)
  {
    IK_PROFILE();
  }

  void Scene::SetViewportSize(uint32_t width, uint32_t height)
  {
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
  
  Entity Scene::TryGetEntityWithUUID(UUID id) const
  {
    if (const auto iter = m_entityIDMap.find(id); iter != m_entityIDMap.end())
    {
      return iter->second;
    }
    return Entity{};
  }

  void Scene::SetName(const std::string &name)
  {
    m_name = name;
  }
  void Scene::SetSelectedEntity(entt::entity entity)
  {
    m_selectedEntity = entity;
  }

  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
  entt::entity Scene::GetSelectedEntity() const
  {
    return m_selectedEntity;
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

} // namespace IKan
