//
//  PhysicsJoint.cpp
//  IKan
//
//  Created by Ashish . on 08/09/23.
//

#include "PhysicsJoint.hpp"

namespace IKan
{
  PhysicsJoint::PhysicsJoint(PhysicsWorld* world, RigidBody *body1, RigidBody *body2, bool worldSpace)
  : world(world), body1(body1), body2(body2), worldSpace(worldSpace)
  {
    
  }
  
  void PhysicsJoint::MakeFixed(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2)
  {
    if (worldSpace)
    {
      FixedJointInfo jointInfo(body1, body2, worldAnchorPoint);
      world->createJoint(jointInfo);
    }
    else
    {
      FixedJointInfo jointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2);
      world->createJoint(jointInfo);
    }
  }
  
  void PhysicsJoint::MakeBallSocket(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2)
  {
    if (worldSpace)
    {
      BallAndSocketJointInfo jointInfo(body1, body2, worldAnchorPoint);
      world->createJoint(jointInfo);
    }
    else
    {
      BallAndSocketJointInfo jointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2);
      world->createJoint(jointInfo);
    }
  }
} // namespace IKan
