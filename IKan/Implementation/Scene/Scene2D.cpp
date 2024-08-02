//
//  Scene2D.cpp
//  IKan
//
//  Created by Ashish . on 02/08/24.
//

#include "Scene2D.hpp"

namespace IKan
{
  Scene2D::Scene2D(const std::string& name, uint32_t maxEntityCapacity)
  : Scene(SceneType::_2D, name, maxEntityCapacity)
  {
    IK_ASSERT(false, "Implement later");
  }
  
  Scene2D::~Scene2D()
  {
  }
} // namespace IKan
