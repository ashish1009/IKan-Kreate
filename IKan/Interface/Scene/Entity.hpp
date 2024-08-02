//
//  Entity.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace IKan
{
  /// This class stores the entity data for scene
  class Entity
  {
  public:
    /// This is the default Contructor for Entity
    Entity() = default;
    /// This is the Scene Contructor for Entity
    /// - Parameters:
    ///   - handle: Entity Handle
    ///   - scene: Scene context
    Entity(entt::entity handle, Scene* scene);
    /// This is the Default Destructor for Entity
    ~Entity() = default;
    
    // Component Manager --------------------------------------------------------------------------------------------
    /// This function adds component in Current Entity
    /// - Parameter args: Arguments needed to construct the component NOTE : T is type of component
    template<typename T, typename... Args> T& AddComponent(Args&&... args)
    {
      IK_ASSERT(!HasComponent<T>(), "Entity already has component!");
      return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
    }
    
    /// This function returns component from Current Entity NOTE : T is type of component
    template<typename T> T& GetComponent()
    {
      IK_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
      return m_scene->m_registry.get<T>(m_entityHandle);
    }
    
    /// This function returns component from Current Entity NOTE : T is type of component
    template<typename T> const T& GetComponent() const
    {
      IK_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
      return m_scene->m_registry.get<T>(m_entityHandle);
    }
    
    /// This function checks if Entity have a component return true if present NOTE : T is type of component
    template<typename... T> bool HasComponent()
    {
      return m_scene->m_registry.has<T...>(m_entityHandle);
    }
    
    /// This function checks if Entity have a component return true if present NOTE : T is type of component
    template<typename... T> bool HasComponent() const
    {
      return m_scene->m_registry.has<T...>(m_entityHandle);
    }
    
    /// This function returns if entity have any component
    template<typename...T> bool HasAny()
    {
      return m_scene->m_registry.any<T...>(m_entityHandle);
    }
    
    /// This function returns if entity have any component
    template<typename...T> bool HasAny() const
    {
      return m_scene->m_registry.any<T...>(m_entityHandle);
    }
    
    /// This function removes component from Current Entity NOTE : T is type of component
    template<typename T> void RemoveComponent()
    {
      IK_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
      m_scene->m_registry.remove<T>(m_entityHandle);
    }

  private:
    entt::entity m_entityHandle{ entt::null };
    Scene* m_scene = nullptr;
    friend class Scene;
  };
} // namespace IKan
