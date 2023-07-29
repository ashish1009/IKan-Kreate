//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <entt/entt.hpp>
#include "Core/TimeStep.hpp"
#include "Camera/EditorCamera.hpp"

namespace IKan
{
  class Entity;
  
  class Scene
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene(uint32_t maxEntityCapacity = 200000);
    /// This is the default destructor of EnTT Scene
    ~Scene();
    
    /// This function creates the instance of EnTT Scene
    /// - Parameters:
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    static Ref<Scene> Create(uint32_t maxEntityCapacity = 200000);
    
  private:
    entt::registry m_registry;
    uint32_t m_registryCapacity = 0;

    friend class Entity;
  };
} // namespace IKan
