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
  
  void PhysicsJoint::MakeBallSocket(bool coneLimit, float coneAngle)
  {
    BallAndSocketJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = iknew BallAndSocketJointInfo(body1, body2, worldAnchorPoint);
    }
    else
    {
      jointInfo = iknew BallAndSocketJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2);
    }

    BallAndSocketJoint* joint = dynamic_cast<BallAndSocketJoint*>(world->createJoint(*jointInfo));
    joint->enableConeLimit(coneLimit);
    joint->setConeLimitHalfAngle(coneAngle);

    ikdelete jointInfo;
  }
  
  void PhysicsJoint::MakeHinge(const Vector3& worldAxis, const Vector3& localAxis1, const Vector3& localAxis2,
                               bool limit, decimal initMinAngleLimit, decimal initMaxAngleLimit,
                               bool motor, decimal initMotorSpeed, decimal initMaxMotorTorque)
  {
    HingeJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = iknew HingeJointInfo(body1, body2, worldAnchorPoint, worldAxis);
    }
    else
    {
      jointInfo = iknew HingeJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2, localAxis1, localAxis2);
    }
    
    jointInfo->isLimitEnabled = limit;
    if (limit)
    {
      jointInfo->minAngleLimit = initMinAngleLimit;
      jointInfo->maxAngleLimit = initMaxAngleLimit;
    }

    jointInfo->isMotorEnabled = motor;
    if (motor)
    {
      jointInfo->motorSpeed = initMotorSpeed;
      jointInfo->maxMotorTorque = initMaxMotorTorque;
    } 

    [[maybe_unused]] HingeJoint* joint = dynamic_cast<HingeJoint*>(world->createJoint(*jointInfo));
    ikdelete jointInfo;
  }

  void PhysicsJoint::MakeSlider(const Vector3& worldAxis, const Vector3& localAxis1,
                                bool limit, decimal initMinTransLimit, decimal initMaxTransLimit,
                                bool motor, decimal initMotorSpeed, decimal initMaxMotorForce)
  {
    SliderJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = iknew SliderJointInfo(body1, body2, worldAnchorPoint, worldAxis);
    }
    else
    {
      jointInfo = iknew SliderJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2, localAxis1);
    }
    
    jointInfo->isLimitEnabled = limit;
    if (limit)
    {
      jointInfo->minTranslationLimit = initMinTransLimit;
      jointInfo->maxTranslationLimit = initMaxTransLimit;
    }
    
    jointInfo->isMotorEnabled = motor;
    if (motor)
    {
      jointInfo->motorSpeed = initMotorSpeed;
      jointInfo->maxMotorForce = initMaxMotorForce;
    }
    
    [[maybe_unused]] SliderJoint* joint = dynamic_cast<SliderJoint*>(world->createJoint(*jointInfo));
    ikdelete jointInfo;
  }

} // namespace IKan
