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
} // namespace IKan
