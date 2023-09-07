//
//  PhysicsScene.hpp
//  IKan
//
//  Created by Ashish . on 07/09/23.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Core/TimeStep.hpp"

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
    
    /// This function handles the physics update
    /// - Parameter ts: time step
    void OnUpdate(TimeStep ts);
    
    /// This function returns the physics Debug renderer
    reactphysics3d::DebugRenderer GetDebugRenderer() const;

  private:
    PhysicsSettings m_physicsSettings;

    reactphysics3d::PhysicsCommon m_physics3DCommon;
    reactphysics3d::PhysicsWorld* m_physics3DWorld = nullptr;
    
    Scene* m_scene;
  };
} // namespace IKan
