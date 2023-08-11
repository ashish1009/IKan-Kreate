//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Renderer/Graphics/Pipeline.hpp"

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
    
    /// This function initializes the Scene renderer
    void Initialize();
    
  private:
    Ref<Scene> m_scene;
    Ref<Pipeline> m_geometryPipeline;
  };
} // namespace IKan
