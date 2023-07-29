//
//  Scene.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <entt/entt.hpp>
#include "Core/TimeStep.hpp"
#include "Camera/EditorCamera.hpp"

namespace IKan
{
  class Scene
  {
  public:
    /// This is the default constructor of EnTT Scene
    Scene();
    /// This is the default destructor of EnTT Scene
    ~Scene();
    
    /// This function creates the instance of EnTT Scene
    static Ref<Scene> Create();
  };
} // namespace IKan
