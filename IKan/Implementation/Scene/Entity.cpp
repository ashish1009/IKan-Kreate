//
//  Entity.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "Entity.hpp"

namespace IKan
{
  Entity::Entity(entt::entity handle, Scene* scene)
  : m_entityHandle(handle), m_scene(scene)
  {
    
  }
} // namespace IKan
