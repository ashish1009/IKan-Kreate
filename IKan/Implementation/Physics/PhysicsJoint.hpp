//
//  PhysicsJoint.hpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#pragma once

#include "PhysicsScene.hpp"

using namespace reactphysics3d;

namespace IKan
{
  struct PhysicsJoint
  {
    bool worldSpace = false;
    bool allowCollision = false;
    reactphysics3d::PhysicsWorld* world = nullptr;
    reactphysics3d::RigidBody *body1, *body2;
    Vector3 worldAnchorPoint;
    Vector3 localAnchorPoint1;
    Vector3 localAnchorPoint2;
    
    PhysicsJoint(PhysicsWorld* world, RigidBody *body1, RigidBody *body2, bool worldSpace = false, bool allowCollision = false);
    
    /// This function set the anchors
    /// - Parameters:
    ///   - worldAnchorPoint: worldAnchorPoint
    ///   - localAnchorPoint1: localAnchorPoint1
    ///   - localAnchorPoint2: localAnchorPoint2
    void SetAnchors(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2);
    
    /// This function create the fixed joint in world
    void MakeFixed();
    
    /// This function create the fixed joint in world
    /// - Parameters:
    ///   - coneLimit: Cone Limit flag
    ///   - coneAngle: cone angle
    void MakeBallSocket(bool coneLimit, float coneAngle);
    
    /// This function create the fixed joint in world
    /// - Parameters:
    ///   - worldAxis: worldAxis
    ///   - localAxis1: localAxis1
    ///   - localAxis2: localAxis2
    ///   - initMinAngleLimit: initMinAngleLimit
    ///   - initMaxAngleLimit: initMaxAngleLimit
    ///   - initMotorSpeed: initMotorSpeed
    ///   - initMaxMotorTorque: initMaxMotorTorque
    ///   - limit: Limit flag
    ///   - motor: motor flag
    void MakeHinge(const Vector3& worldAxis, const Vector3& localAxis1, const Vector3& localAxis2,
                   bool limit, decimal initMinAngleLimit, decimal initMaxAngleLimit,
                   bool motor, decimal initMotorSpeed, decimal initMaxMotorTorque);
    
    /// This function create the fixed joint in world
    /// - Parameters:
    ///   - worldAxis: worldAxis
    ///   - localAxis1: localAxis
    ///   - initMinAngleLimit: initMinAngleLimit
    ///   - initMaxAngleLimit: initMaxAngleLimit
    ///   - initMotorSpeed: initMotorSpeed
    ///   - initMaxMotorForce: initMaxMotorTorque
    ///   - limit: Limit flag
    ///   - motor: motor flag
    void MakeSlider(const Vector3& worldAxis, const Vector3& localAxis1,
                    bool limit, decimal initMinTransLimit, decimal initMaxTransLimit,
                    bool motor, decimal initMotorSpeed, decimal initMaxMotorForce);
  };
} // namespace IKan
