//
//  PhysicsScene.hpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Core/TimeStep.hpp"
#include "Physics/ContactListener.hpp"

namespace IKan
{
  class Scene;

  struct PhysicsSettings
  {
    glm::vec3 gravity = { 0.0f, -9.81f, 0.0f };
    bool isAllowSleep = true;
    uint32_t solverPositionIterations = 8;
    uint32_t solverVelocityIterations = 2;
  };
  
  class PhysicsScene
  {
  public:
    /// This constructor creates the physics scene
    /// - Parameters:
    ///   - setting: physics setting
    ///   - scene: Scene reference
    PhysicsScene(const PhysicsSettings& setting, Scene* scene);
    /// This destructor destroy the physics scene
    ~PhysicsScene();
    
  private:
    PhysicsSettings m_settings;
    
    reactphysics3d::PhysicsCommon m_common;
    reactphysics3d::PhysicsWorld* m_world = nullptr;
    
    Scene* m_scene;
    ContactListener m_contactListener;
  };
} // namespace IKan
