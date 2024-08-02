//
//  Scene.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "Scene.hpp"

#include "Scene/Component.hpp"
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

  void Scene::SetName(const std::string &name)
  {
    m_name = name;
  }

  const std::string& Scene::GetName() const
  {
    return m_name;
  }
  entt::registry& Scene::GetRegistry()
  {
    return m_registry;
  }
} // namespace IKan
