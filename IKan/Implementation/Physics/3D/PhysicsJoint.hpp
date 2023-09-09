//
//  PhysicsJoint.hpp
//  IKan
//
//  Created by Ashish . on 08/09/23.
//

#pragma once

#include "PhysicsScene.hpp"

using namespace reactphysics3d;

namespace IKan
{
  struct PhysicsJoint
  {
    bool worldSpace = false;
    reactphysics3d::PhysicsWorld* world = nullptr;
    reactphysics3d::RigidBody *body1, *body2;
    Vector3 worldAnchorPoint;
    Vector3 localAnchorPoint1;
    Vector3 localAnchorPoint2;
    
    PhysicsJoint(PhysicsWorld* world, RigidBody *body1, RigidBody *body2, bool worldSpace = false);
    
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
    ///   - axis : axis of movement
    ///   - worldAxis: worldAxis
    ///   - localAxis1: localAxis1
    ///   - localAxis2: localAxis2
    ///   - initMinAngleLimit: initMinAngleLimit
    ///   - initMaxAngleLimit: initMaxAngleLimit
    ///   - initMotorSpeed: initMotorSpeed 
    ///   - initMaxMotorTorque: initMaxMotorTorque 
    void MakeHinge(const Vector3& worldAxis, const Vector3& localAxis1, const Vector3& localAxis2,
                   decimal initMinAngleLimit, decimal initMaxAngleLimit, decimal initMotorSpeed,
                   decimal initMaxMotorTorque);
  };
} // namespace IKan
