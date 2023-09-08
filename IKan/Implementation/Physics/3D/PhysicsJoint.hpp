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
    
    PhysicsJoint(PhysicsWorld* world, RigidBody *body1, RigidBody *body2, bool worldSpace = false);
    
    /// This function create the fixed joint in world
    /// - Parameters:
    ///   - worldAnchorPoint: worldAnchorPoint
    ///   - localAnchorPoint1: localAnchorPoint1
    ///   - localAnchorPoint2: localAnchorPoint2
    void MakeFixed(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2);

    /// This function create the fixed joint in world
    /// - Parameters:
    ///   - worldAnchorPoint: worldAnchorPoint
    ///   - localAnchorPoint1: localAnchorPoint1
    ///   - localAnchorPoint2: localAnchorPoint2
    void MakeBallSocket(const Vector3& worldAnchorPoint, const Vector3& localAnchorPoint1, const Vector3& localAnchorPoint2);
  };
} // namespace IKan
