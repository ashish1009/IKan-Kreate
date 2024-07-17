//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"

namespace IKan
{
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
    
    /// This function updates the viewport size of scene rendere data
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewportSize(uint32_t width, uint32_t height);

  private:
    std::string m_debugName {"IKan Renderer"}; 
    uint32_t m_viewportWidth {0}, m_viewportHeight {0};

    Ref<FrameBuffer> m_geometryRenderPass;
  };
} // namespace IKan
