//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Camera/Camera.hpp"

namespace IKan
{
  /// Scene renderer Camera Data
  struct SceneRendererCamera
  {
    Camera camera;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    float near, far; //Non-reversed
    float FOV;
  };

  /// This class is responsible for Rendering Scene in Viewport
  class SceneRenderer
  {
  public:
    /// This is the default constructor of Scene Renderer
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(const std::string& debugName);
    /// This is the default destructor of Scene Renderer
    ~SceneRenderer();

    /// This funciton begins the scene renderering
    /// - Parameter sceneCamera: Scene Renderer camera data
    void BeginScene(const SceneRendererCamera& sceneCamera);
    /// This funciton ends the scene renderering
    void EndScene();

    /// This function updates the viewport size of scene rendere
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewportSize(uint32_t width, uint32_t height);

    /// This function returns the final render pass image
    Ref<Texture> GetFinalImage() const;

  private:
    // Member functions ---------------------------------------------------------------------------------------------

    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_debugName;
    uint32_t m_viewportWidth, m_viewportHeight;
    
    Ref<FrameBuffer> m_viewportRenderPass;
  };
} // namespace IKan
