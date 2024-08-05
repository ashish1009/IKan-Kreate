//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"
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

  /// This class stores the API to submit the shapes to render them each frame
  /// - Note: This class renderes each shape inside a framebuffer
  class SceneRenderer
  {
  public:
    /// This Constructore creaates the scene renderer instance
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(std::string_view debugName);
    /// This Destrcutro destroys the scene renderer instance
    ~SceneRenderer();
    
    /// This funciton begins the scene renderering
    /// - Parameters:
    ///   - sceneCamera: Scene Renderer camera data
    void BeginScene(const SceneRendererCamera& sceneCamera);
    /// This funciton ends the scene renderering and flush the rendering data for frame
    void EndScene();
    
    /// This function updates the viewport size of scene rendere data
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewportSize(uint32_t width, uint32_t height);

    /// This funcion set the callback for debug renderer
    /// - Parameter func: callback funciton
    void SetDebugRenderer(const std::function<void()>& func);

    /// This function returns the final render pass image
    Ref<Texture> GetFinalImage() const;

  private:
    std::string m_debugName {"IKan Renderer"}; 
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};

    // Debug Renderer from client
    std::function<void()> m_debugRenderer = []() {};

    Ref<FrameBuffer> m_geometryRenderPass;
  };
} // namespace IKan
