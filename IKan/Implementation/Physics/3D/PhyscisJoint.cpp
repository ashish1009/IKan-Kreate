//
//  PhyscisJoint.cpp
//  IKan
//
//  Created by Ashish . on 07/09/23.
//

#include "PhyscisJoint.hpp"

namespace IKan
{
  JointBase::JointBase(Entity entity, Entity connectedEntity, JointType type)
  : m_connectedEntity(connectedEntity), m_type(type)
  {
    
  }
  
  FixedJoint::FixedJoint(Entity entity, Entity connectedEntity)
  : JointBase(entity, connectedEntity, JointType::Fixed)
  {
    
  }
  
  FixedJoint::~FixedJoint()
  {
    
  }
} // namespace IKan
