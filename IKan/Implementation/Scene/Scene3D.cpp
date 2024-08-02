//
//  Scene3D.cpp
//  IKan
//
//  Created by Ashish . on 02/08/24.
//

#include "Scene3D.hpp"

namespace IKan
{
  Scene3D::Scene3D(const std::string& name, uint32_t maxEntityCapacity)
  : Scene(SceneType::_3D, name, maxEntityCapacity)
  {
  }
  
  Scene3D::~Scene3D()
  {
  }
} // namespace IKan
