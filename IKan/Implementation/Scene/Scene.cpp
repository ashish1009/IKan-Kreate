//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "Scene.hpp"
#include "Scene/Components.hpp"

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

  void Scene::SetName(const std::string &name)
  {
    m_name = name;
  }

  const std::string& Scene::GetName() const
  {
    return m_name;
  }

} // namespace IKan {
