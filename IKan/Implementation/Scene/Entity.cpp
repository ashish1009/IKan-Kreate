//
//  Entity.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Entity.hpp"

namespace IKan
{
  Entity::Entity(entt::entity handle, Scene* scene)
  : m_entityHandle(handle), m_scene(scene)
  {
    
  }
    
  TransformComponent& Entity::Transform()
  {
    return m_scene->m_registry.get<TransformComponent>(m_entityHandle);
  }
  const glm::mat4& Entity::Transform() const
  {
    return m_scene->m_registry.get<TransformComponent>(m_entityHandle).Transform();
  }
  
  std::string& Entity::Name()
  {
    return HasComponent<TagComponent>() ? GetComponent<TagComponent>().tag : s_noName;
  }
  const std::string& Entity::Name() const
  {
    return HasComponent<TagComponent>() ? GetComponent<TagComponent>().tag : s_noName;
  }

  UUID Entity::GetParentUUID() const
  {
    return GetComponent<RelationshipComponent>().parentHandle;
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
  
  UUID Entity::GetUUID() const
  {
    return GetComponent<IDComponent>().ID;
  }
} // namespace IKan
