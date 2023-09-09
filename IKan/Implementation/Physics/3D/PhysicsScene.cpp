//
//  PhysicsScene.cpp
//  IKan
//
//  Created by Ashish . on 07/09/23.
//

#include "PhysicsScene.hpp"
#include "PhysicsJoint.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Entity.hpp"

using namespace reactphysics3d;

namespace IKan
{
  PhysicsScene::PhysicsScene(const PhysicsSettings& setting, Scene* scene)
  : m_settings(setting), m_scene(scene)
  {
    IK_LOG_TRACE(LogModule::Physics, "Creating Physics Scene");
    IK_LOG_TRACE(LogModule::Physics, "  Gravity {0} | {1} | {2}",
                 setting.gravity.x, setting.gravity.y, setting.gravity.z);
    
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
    
    auto rigidBodyView = m_scene->GetAllEntitiesWith<RigidBodyComponent>();
    for (auto entityHandle : rigidBodyView)
    {
      Entity entity = { entityHandle, m_scene };
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
      body->setLinearDamping(rbc.liniarDamping);
      body->setAngularDamping(rbc.angularDamping);
      body->setIsAllowedToSleep(rbc.allowSleep);
            
      // Box 3D -----------------------------------------------------------------------------------------------------
      if (entity.HasComponent<Box3DColliderComponent>())
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
        
        // Get the current material of the collider
        Material& material = collider->getMaterial();
        
        material.setBounciness(bcc.bounciness);
        material.setFrictionCoefficient(bcc.frictionCoefficient);
        material.setMassDensity(bcc.massDensity);
      } // Box3d Collider
      
      // Sphere -----------------------------------------------------------------------------------------------------
      if (entity.HasComponent<SphereColliderComponent>())
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
        
        // Get the current material of the collider
        Material& material = collider->getMaterial();
        
        material.setBounciness(scc.bounciness);
        material.setFrictionCoefficient(scc.frictionCoefficient);
        material.setMassDensity(scc.massDensity);
      } // Sphere Collider
      
      // Capsule -----------------------------------------------------------------------------------------------------
      if (entity.HasComponent<CapsuleColliderComponent>())
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
        
        // Get the current material of the collider
        Material& material = collider->getMaterial();
        
        material.setBounciness(ccc.bounciness);
        material.setFrictionCoefficient(ccc.frictionCoefficient);
        material.setMassDensity(ccc.massDensity);
      } // Capsule Collider
    } // Each Rigid Body
    
    // Joints -------------------------------------------------------------------------------------
    auto view = m_scene->GetAllEntitiesWith<JointComponent>();
    for (auto entity : view)
    {
      Entity e = { entity, m_scene };
      CreateJoint(e);
    }
  }
  
  PhysicsScene::~PhysicsScene()
  {
    IK_LOG_WARN(LogModule::Physics, "Destroying Physics Scene");
    m_common.destroyPhysicsWorld(m_world);
  }
  
  void PhysicsScene::OnUpdate(TimeStep ts)
  {
    // Change the number of iterations of the velocity solver
    m_world->setNbIterationsVelocitySolver(15);
    // Change the number of iterations of the position solver
    m_world->setNbIterationsPositionSolver(8);
    
    // Update the Dynamics world with a constant time step
    m_world->update(ts);
    
    // Get Transform
    auto view = m_scene->GetAllEntitiesWith<RigidBodyComponent>();
    for (auto entityHandle : view)
    {
      Entity entity = { entityHandle, m_scene };
      
      auto& rbc = entity.GetComponent<RigidBodyComponent>();
      if (rbc.bodyType == RigidBodyComponent::BodyType::Dynamic or rbc.bodyType == RigidBodyComponent::BodyType::Kinametic)
      {
        auto& tc = entity.GetComponent<TransformComponent>();
        RigidBody* body = (RigidBody*)rbc.runtimeBody;
        if (body != nullptr)
        {
          const auto& tramsform = body->getTransform();
          tc.UpdatePosition({tramsform.getPosition().x, tramsform.getPosition().y, tramsform.getPosition().z});
          
          glm::quat q(tramsform.getOrientation().w,
                      tramsform.getOrientation().x,
                      tramsform.getOrientation().y,
                      tramsform.getOrientation().z);
          
          const glm::vec3 angles = glm::eulerAngles(q);
          tc.UpdateRotation(angles);
        }
      } // if (rb2d.type == b2_dynamicBody or rb2d.type == b2_kinematicBody)
    } // for (auto e : view)
  }
  
  void PhysicsScene::CreateJoint(Entity entity)
  {
    const auto& fjc = entity.GetComponent<JointComponent>();
    Entity connectedEntity = m_scene->GetEntityWithUUID(fjc.connectedEntity);

    const auto& rigidBodyComponent1 = entity.GetComponent<RigidBodyComponent>();
    auto body1 = static_cast<RigidBody*>(rigidBodyComponent1.runtimeBody);
    
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
  
  DebugRenderer PhysicsScene::GetDebugRenderer() const
  {
    return m_world->getDebugRenderer();
  }
} // namespace IKan
