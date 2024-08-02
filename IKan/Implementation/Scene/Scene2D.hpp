//
//  Scene2D.hpp
//  IKan
//
//  Created by Ashish . on 02/08/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace IKan
{
  /// This class stores the 2D scene data
  class Scene2D : public Scene
  {
  public:
    /// This is the default constructor of EnTT Scene
    /// - Parameters:
    ///   - name: Name of Scene
    ///   - maxEntityCapacity: Max Entity capacity to reserve
    Scene2D(const std::string& name, uint32_t maxEntityCapacity);
    /// This is the default destructor of EnTT Scene
    ~Scene2D();
  };
} // namespace IKan
