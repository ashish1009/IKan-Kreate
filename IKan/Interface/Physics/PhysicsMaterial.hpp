//
//  PhysicsMaterial.hpp
//  IKan
//
//  Created by Ashish . on 03/03/24.
//

#pragma once

#include "Asset/Asset.hpp"

namespace IKan
{
  class PhysicsMaterial : public Asset
  {
  public:
    float frictionCoefficient = 0.2;
    float bounciness = 0.1;
    float massDensity = 1.0;

    PhysicsMaterial() = default;
    PhysicsMaterial(float frictionCoefficient, float bounciness, float massDensity)
    : frictionCoefficient(frictionCoefficient), bounciness(bounciness), massDensity(massDensity)
    {
    }
 
    ASSET_TYPE(PhysicsMaterial)
  };
} // namespace IKan
