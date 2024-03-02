//
//  PhysicsScene.hpp
//  IKan
//
//  Created by Ashish . on 02/03/24.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Core/TimeStep.hpp"

namespace IKan
{
  struct PhysicsSettings
  {
    glm::vec3 gravity = { 0.0f, -9.81f, 0.0f };
    bool isAllowSleep = true;
    uint32_t solverPositionIterations = 8;
    uint32_t solverVelocityIterations = 2;
  };
} // namespace IKan
