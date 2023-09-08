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
  
  void PhysicsJoint::SetAnchors(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2)
  {
    this->worldAnchorPoint = worldAnchorPoint;
    this->localAnchorPoint1 = localAnchorPoint1;
    this->localAnchorPoint2 = localAnchorPoint2;
  }
  
  void PhysicsJoint::MakeFixed()
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
  
  void PhysicsJoint::MakeBallSocket()
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
  
  void PhysicsJoint::MakeHinge(const Vector3& worldAxis, const Vector3& localAxis1, const Vector3& localAxis2,
                               decimal initMinAngleLimit, decimal initMaxAngleLimit, decimal initMotorSpeed,
                               decimal initMaxMotorTorque)
  {
    if (worldSpace)
    {
      HingeJointInfo jointInfo(body1, body2, worldAnchorPoint, worldAxis,
                               initMinAngleLimit, initMaxAngleLimit, initMotorSpeed, initMaxMotorTorque);
      world->createJoint(jointInfo);
    }
    else
    {
      HingeJointInfo jointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2,
                               initMinAngleLimit, initMaxAngleLimit, initMotorSpeed, initMaxMotorTorque);

      world->createJoint(jointInfo);
    }
  }

} // namespace IKan
