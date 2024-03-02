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
  class Entity;
  class RigidBodyComponent;
  class Box3DColliderComponent;
  class SphereColliderComponent;
  class CapsuleColliderComponent;
  class JointComponent;

  struct PhysicsSettings
  {
    glm::vec3 gravity = { 0.0f, -9.81f, 0.0f };
    bool isAllowSleep = true;
    uint32_t solverPositionIterations = 8;
    uint32_t solverVelocityIterations = 2;
  };
    
  enum class ColliderType
  {
    Box, Sphere, Capsule
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
    const reactphysics3d::DebugRenderer& GetDebugRenderer() const;

  private:
    /// This function creates new joint in the entity
    /// - Parameter entity: entity
    void CreateJoint(Entity entity);

    PhysicsSettings m_settings;
    
    reactphysics3d::PhysicsCommon m_common;
    reactphysics3d::PhysicsWorld* m_world = nullptr;
    
    Scene* m_scene;
    ContactListener m_contactListener;
  };
} // namespace IKan
