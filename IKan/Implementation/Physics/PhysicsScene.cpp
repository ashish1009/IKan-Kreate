//
//  PhysicsScene.cpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#include "PhysicsScene.hpp"
#include "Physics/PhysicsJoint.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Components.hpp"

using namespace reactphysics3d;

namespace IKan
{
  PhysicsScene::PhysicsScene(const PhysicsSettings& setting, Scene* scene)
  : m_settings(setting), m_scene(scene)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Physics, "Creating Physics Scene");
    IK_LOG_TRACE(LogModule::Physics, "  Gravity {0} | {1} | {2}", setting.gravity.x, setting.gravity.y, setting.gravity.z);
    
    // Create the world settings
    PhysicsWorld::WorldSettings settings;
    settings.defaultVelocitySolverNbIterations = m_settings.solverVelocityIterations;
    settings.defaultPositionSolverNbIterations = m_settings.solverPositionIterations;
    settings.isSleepingEnabled = m_settings.isAllowSleep;
    settings.gravity = Vector3(m_settings.gravity.x, m_settings.gravity.y, m_settings.gravity.z);
    
    // Create the physics world with your settings
    m_world = m_common.createPhysicsWorld(settings);
    
    // Debug Renderer
    m_world->setIsDebugRenderingEnabled(true);
    
    // Get a reference to the debug renderer
    DebugRenderer& debugRenderer = m_world->getDebugRenderer();
    debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    
    // Override contact listener
    m_world->setEventListener(&m_contactListener);
  }
  
  PhysicsScene::~PhysicsScene()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Physics, "Destroying Physics Scene");
    m_common.destroyPhysicsWorld(m_world);
  }
  
  void PhysicsScene::AddBody(Entity entity)
  {
    IK_PROFILE();
    auto& rbc = entity.GetComponent<RigidBodyComponent>();
    auto& tc = entity.GetComponent<TransformComponent>();
    
    // Initial position and orientation of the rigid body
    Vector3 position (tc.Position().x, tc.Position().y, tc.Position().z);
    auto quaternion = glm::quat(tc.Rotation());
    Quaternion orientation(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
    
    // Create a rigid body in the world
    Transform transform(position, orientation);
    RigidBody* body = m_world->createRigidBody(transform);
    rbc.runtimeBody = body;
    
    // Change the body parameters
    body->setType(RigidBodyComponent::ReactPhysicsBodyType(rbc.bodyType));
    body->enableGravity(rbc.enableGravity);
    body->setLinearDamping(rbc.linearDamping);
    body->setAngularDamping(rbc.angularDamping);
    body->setIsAllowedToSleep(rbc.allowSleep);
    body->setAngularLockAxisFactor({rbc.angularAxisMove.x, rbc.angularAxisMove.y, rbc.angularAxisMove.z});
  }
  void PhysicsScene::AddCollider(ColliderType type, Entity entity)
  {
    IK_PROFILE();
    auto& rbc = entity.GetComponent<RigidBodyComponent>();
    auto& tc = entity.GetComponent<TransformComponent>();

    RigidBody* body = (RigidBody*)rbc.runtimeBody;

    switch (type)
    {
      case ColliderType::Box:
      {
        auto& bcc = entity.GetComponent<Box3DColliderComponent>();
        // Half extents of the box in the x, y and z directions
        glm::vec3 relativeSize = tc.Scale() * bcc.size;
        const Vector3 halfExtents(relativeSize.x, relativeSize.y, relativeSize.z);
        
        // Create the box shape
        BoxShape* boxShape = m_common.createBoxShape(halfExtents);
        
        // Add the collider to the rigid body
        auto colliderPosition = Vector3(bcc.positionOffset.x, bcc.positionOffset.y, bcc.positionOffset.x);
        auto collideerQuaternion = Quaternion(bcc.quaternionOffset.x, bcc.quaternionOffset.y, bcc.quaternionOffset.z,
                                              bcc.quaternionOffset.w);
        Transform collidertransform = Transform(colliderPosition, collideerQuaternion);
        Collider* collider = body->addCollider(boxShape, collidertransform);
        
        // NOTE: To delete this while deleting the collider
        bcc.userData = new Entity(entity);
        collider->setUserData(bcc.userData);
        
        // Get the current material of the collider
        reactphysics3d::Material& material = collider->getMaterial();
        
        material.setBounciness(bcc.bounciness);
        material.setFrictionCoefficient(bcc.frictionCoefficient);
        material.setMassDensity(bcc.massDensity);
        
        body->updateMassPropertiesFromColliders();
        break;
      }
        
      case ColliderType::Sphere:
      {
        auto& scc = entity.GetComponent<SphereColliderComponent>();
        
        // Half extents of the box in the x, y and z directions
        glm::vec3 relativeRadius = tc.Scale() * scc.radius;
        
        // Create the box shape
        // NOTE: There is no feature to have oval shape sp size should be same for all xyz
        SphereShape* boxShape = m_common.createSphereShape(relativeRadius.x);
        
        // Add the collider to the rigid body
        auto colliderPosition = Vector3(scc.positionOffset.x, scc.positionOffset.y, scc.positionOffset.x);
        auto collideerQuaternion = Quaternion(scc.quaternionOffset.x, scc.quaternionOffset.y, scc.quaternionOffset.z,
                                              scc.quaternionOffset.w);
        Transform collidertransform = Transform(colliderPosition, collideerQuaternion);
        Collider* collider = body->addCollider(boxShape, collidertransform);
        
        // NOTE: To delete this while deleting the collider
        scc.userData = new Entity(entity);
        collider->setUserData(scc.userData);
        
        // Get the current material of the collider
        reactphysics3d::Material& material = collider->getMaterial();
        
        material.setBounciness(scc.bounciness);
        material.setFrictionCoefficient(scc.frictionCoefficient);
        material.setMassDensity(scc.massDensity);
        break;
      }
        
      case ColliderType::Capsule:
      {
        auto& ccc = entity.GetComponent<CapsuleColliderComponent>();
        
        // Half extents of the box in the x, y and z directions
        glm::vec3 relativeRadius = tc.Scale() * ccc.radius;
        glm::vec3 relativeSize = tc.Scale() * ccc.height;
        
        // Create the capsule shape
        CapsuleShape* capsuleShape = m_common.createCapsuleShape(relativeRadius.x, relativeSize.y);
        
        // Add the collider to the rigid body
        auto colliderPosition = Vector3(ccc.positionOffset.x, ccc.positionOffset.y, ccc.positionOffset.x);
        auto collideerQuaternion = Quaternion(ccc.quaternionOffset.x, ccc.quaternionOffset.y, ccc.quaternionOffset.z,
                                              ccc.quaternionOffset.w);
        Transform collidertransform = Transform(colliderPosition, collideerQuaternion);
        Collider* collider = body->addCollider(capsuleShape, collidertransform);
        
        // NOTE: To delete this while deleting the collider
        ccc.userData = new Entity(entity);
        collider->setUserData(ccc.userData);
        
        // Get the current material of the collider
        reactphysics3d::Material& material = collider->getMaterial();
        
        material.setBounciness(ccc.bounciness);
        material.setFrictionCoefficient(ccc.frictionCoefficient);
        material.setMassDensity(ccc.massDensity);
        break;
      }
      default:
        break;
    }

  }
  void PhysicsScene::CreateJoint(Entity entity)
  {
    IK_PROFILE();
    const auto& fjc = entity.GetComponent<JointComponent>();
    Entity connectedEntity = m_scene->GetEntityWithUUID(fjc.connectedEntity);
    
    const auto& rigidBodyComponent1 = entity.GetComponent<RigidBodyComponent>();
    auto body1 = static_cast<RigidBody*>(rigidBodyComponent1.runtimeBody);
    
    IK_ASSERT(connectedEntity.HasComponent<RigidBodyComponent>());
    const auto& rigidBodyComponent2 = connectedEntity.GetComponent<RigidBodyComponent>();
    auto body2 = static_cast<RigidBody*>(rigidBodyComponent2.runtimeBody);
    
    PhysicsJoint joint(m_world, body1, body2, fjc.isWorldSpace, fjc.isCollisionEnabled);
    
    // Anchor point in world-space
    Vector3 worldAnchorPoint({ fjc.worldAnchorPoint.x, fjc.worldAnchorPoint.y, fjc.worldAnchorPoint.z});
    Vector3 localAnchorPoint1({ fjc.localAnchorPoint1.x, fjc.localAnchorPoint1.y, fjc.localAnchorPoint1.z});
    Vector3 localAnchorPoint2({ fjc.localAnchorPoint2.x, fjc.localAnchorPoint2.y, fjc.localAnchorPoint2.z});
    
    joint.SetAnchors(worldAnchorPoint, localAnchorPoint1, localAnchorPoint2);
    
    switch (fjc.type)
    {
      case JointComponent::Type::Fixed:
      {
        joint.MakeFixed();
        break;
      }
      case JointComponent::Type::BallSocket:
      {
        joint.MakeBallSocket(fjc.ballSocketData.coneLimit, fjc.ballSocketData.coneAngle);
        break;
      }
      case JointComponent::Type::Hinge:
      {
        Vector3 worldAxis({ fjc.hingeData.worldAxis.x, fjc.hingeData.worldAxis.y, fjc.hingeData.worldAxis.z});
        Vector3 localAxis1({ fjc.hingeData.localAxis1.x, fjc.hingeData.localAxis1.y, fjc.hingeData.localAxis1.z});
        Vector3 localAxis2({ fjc.hingeData.localAxis2.x, fjc.hingeData.localAxis2.y, fjc.hingeData.localAxis2.z});
        
        joint.MakeHinge(worldAxis, localAxis1, localAxis2,
                        fjc.hingeData.limit, fjc.hingeData.initMinAngleLimit, fjc.hingeData.initMaxAngleLimit,
                        fjc.hingeData.motor, fjc.hingeData.initMotorSpeed, fjc.hingeData.initMaxMotorTorque);
        break;
      }
        
      case JointComponent::Type::Slider:
      {
        Vector3 worldAxis({ fjc.sliderData.worldAxis.x, fjc.sliderData.worldAxis.y, fjc.sliderData.worldAxis.z});
        Vector3 localAxis1({ fjc.sliderData.localAxis1.x, fjc.sliderData.localAxis1.y, fjc.sliderData.localAxis1.z});
        
        joint.MakeSlider(worldAxis, localAxis1,
                         fjc.sliderData.limit, fjc.sliderData.initMinTransLimit, fjc.sliderData.initMaxTransLimit,
                         fjc.sliderData.motor, fjc.sliderData.initMotorSpeed, fjc.sliderData.initMaxMotorForce);
      }
      default:
        break;
    }
  }
} // namespace IKan
