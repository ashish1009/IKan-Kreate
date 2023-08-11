//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
  class SceneRenderer
  {
  public:
    /// This function create scene renderer instance
    /// - Parameter scene: scene context
    SceneRenderer(Ref<Scene> scene);
    /// This destructor destroys the scene renderer instance
    virtual ~SceneRenderer();
  };
} // namespace IKan
