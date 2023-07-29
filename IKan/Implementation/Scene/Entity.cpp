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
  
  void Entity::SetParent(Entity parent)
  {
    Entity currentParent = GetParent();
    if (currentParent == parent)
    {
      return;
    }
    
    // If changing parent, remove child from existing parent
    if (currentParent)
    {
      currentParent.RemoveChild(*this);
    }
    
    // Setting to null is okay
    SetParentUUID(parent.GetUUID());
    
    if (parent)
    {
      auto& parentChildren = parent.Children();
      UUID uuid = GetUUID();
      if (std::find(parentChildren.begin(), parentChildren.end(), uuid) == parentChildren.end())
      {
        parentChildren.emplace_back(GetUUID());
      }
    }
  }
  bool Entity::RemoveChild(Entity child)
  {
    UUID childId = child.GetUUID();
    std::vector<UUID>& children = Children();
    auto it = std::find(children.begin(), children.end(), childId);
    if (it != children.end())
    {
      children.erase(it);
      return true;
    }
    return false;
  }
  void Entity::SetParentUUID(UUID parent)
  {
    GetComponent<RelationshipComponent>().parentHandle = parent;
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
} // namespace IKan
