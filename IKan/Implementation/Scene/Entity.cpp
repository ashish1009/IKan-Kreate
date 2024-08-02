//
//  Entity.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "Entity.hpp"
#include "Scene/Component.hpp"

namespace IKan
{
  static std::string NoName = "Unnamed";

  Entity::Entity(entt::entity handle, Scene* scene)
  : m_entityHandle(handle), m_scene(scene)
  {
    
  }
  
  Entity Entity::GetParent()
  {
    return m_scene->TryGetEntityWithUUID(GetParentUUID());
  }
  UUID Entity::GetParentUUID() const
  {
    return GetComponent<RelationshipComponent>().parentHandle;
  }
  std::vector<UUID>& Entity::Children()
  {
    return GetComponent<RelationshipComponent>().children;
  }
  
  TransformComponent& Entity::GetTransform()
  {
    return m_scene->m_registry.get<TransformComponent>(m_entityHandle);
  }
  const glm::mat4& Entity::GetTransform() const
  {
    return m_scene->m_registry.get<TransformComponent>(m_entityHandle).Transform();
  }
  
  std::string& Entity::GetName()
  {
    return HasComponent<TagComponent>() ? GetComponent<TagComponent>().tag : NoName;
  }
  const std::string& Entity::GetName() const
  {
    return HasComponent<TagComponent>() ? GetComponent<TagComponent>().tag : NoName;
  }
  
  UUID Entity::GetUUID() const
  {
    return GetComponent<IDComponent>().ID;
  }
  
  Entity::operator uint32_t () const
  {
    return (uint32_t)m_entityHandle;
  }
  Entity::operator entt::entity () const
  {
    return m_entityHandle;
  }
  Entity::operator bool () const
  {
    return (m_entityHandle != entt::null) and m_scene;
  }
  
  bool Entity::operator==(const Entity& other) const
  {
    return m_entityHandle == other.m_entityHandle and m_scene == other.m_scene;
  }
  
  bool Entity::operator!=(const Entity& other) const
  {
    return !(*this == other);
  }

} // namespace IKan
