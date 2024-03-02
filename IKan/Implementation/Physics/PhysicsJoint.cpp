//
//  PhysicsJoint.cpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#include "PhysicsJoint.hpp"

namespace IKan
{
  PhysicsJoint::PhysicsJoint(PhysicsWorld* world, RigidBody *body1, RigidBody *body2, bool worldSpace, bool allowCollision)
  : world(world), body1(body1), body2(body2), worldSpace(worldSpace), allowCollision(allowCollision)
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
      jointInfo.isCollisionEnabled = allowCollision;
      world->createJoint(jointInfo);
    }
    else
    {
      FixedJointInfo jointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2);
      jointInfo.isCollisionEnabled = allowCollision;
      world->createJoint(jointInfo);
    }
  }
  
  void PhysicsJoint::MakeBallSocket(bool coneLimit, float coneAngle)
  {
    BallAndSocketJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = new BallAndSocketJointInfo(body1, body2, worldAnchorPoint);
    }
    else
    {
      jointInfo = new BallAndSocketJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2);
    }
    jointInfo->isCollisionEnabled = allowCollision;
    
    BallAndSocketJoint* joint = dynamic_cast<BallAndSocketJoint*>(world->createJoint(*jointInfo));
    joint->enableConeLimit(coneLimit);
    joint->setConeLimitHalfAngle(coneAngle);
    
    delete jointInfo;
  }
  
  void PhysicsJoint::MakeHinge(const Vector3& worldAxis, const Vector3& localAxis1, const Vector3& localAxis2,
                               bool limit, decimal initMinAngleLimit, decimal initMaxAngleLimit,
                               bool motor, decimal initMotorSpeed, decimal initMaxMotorTorque)
  {
    HingeJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = new HingeJointInfo(body1, body2, worldAnchorPoint, worldAxis);
    }
    else
    {
      jointInfo = new HingeJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2, localAxis1, localAxis2);
    }
    jointInfo->isCollisionEnabled = allowCollision;
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
    delete jointInfo;
  }
  
  void PhysicsJoint::MakeSlider(const Vector3& worldAxis, const Vector3& localAxis1,
                                bool limit, decimal initMinTransLimit, decimal initMaxTransLimit,
                                bool motor, decimal initMotorSpeed, decimal initMaxMotorForce)
  {
    SliderJointInfo* jointInfo;
    if (worldSpace)
    {
      jointInfo = new SliderJointInfo(body1, body2, worldAnchorPoint, worldAxis);
    }
    else
    {
      jointInfo = new SliderJointInfo(body1, body2, localAnchorPoint1, localAnchorPoint2, localAxis1);
    }
    jointInfo->isCollisionEnabled = allowCollision;
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
    delete jointInfo;
  }
} // namespace IKan
