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
    
    /// This function adds the rigid body in physics scene
    /// - Parameter entity: Entity
    void AddBody(Entity entity);
    /// This function adds the collider in physics scene
    /// - Parameters:
    ///   - type: collder type
    ///   - body: body pointer
    void AddCollider(ColliderType type, Entity entity);
    /// This function creates new joint in the entity
    /// - Parameter entity: entity
    void CreateJoint(Entity entity);

  private:
    PhysicsSettings m_settings;
    
    reactphysics3d::PhysicsCommon m_common;
    reactphysics3d::PhysicsWorld* m_world = nullptr;
    
    Scene* m_scene;
    ContactListener m_contactListener;
  };
} // namespace IKan
