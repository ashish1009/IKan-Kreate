//
//  PhyscisJoint.hpp
//  IKan
//
//  Created by Ashish . on 07/09/23.
//

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include "Scene/Entity.hpp"

namespace IKan
{
  enum class JointType
  {
    None = -1, Fixed = 0
  };
  
  class JointBase
  {
  public:
  protected:
    JointBase(Entity entity, Entity connectedEntity, JointType type);

    reactphysics3d::Joint* joint;
    Entity m_connectedEntity;
    JointType m_type = JointType::None;
  };
  
  class FixedJoint : public JointBase
  {
  public:
    FixedJoint(Entity entity, Entity connectedEntity);
    virtual ~FixedJoint();
  };
} // namespace IKan
