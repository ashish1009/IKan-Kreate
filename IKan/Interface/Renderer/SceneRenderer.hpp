//
//  SceneRenderer.hpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"

namespace IKan
{
  /// This class is responsible for Rendering Scene in Viewport
  class SceneRenderer
  {
  public:
    /// This is the default constructor of Scene Renderer
    /// - Parameter debugName: debug name of renderer
    SceneRenderer(const std::string& debugName);
    /// This is the default destructor of Scene Renderer
    ~SceneRenderer();

    /// This function updates the viewport size of scene rendere
    /// - Parameters:
    ///   - width: width of viewport
    ///   - height: height of viewport
    void SetViewportSize(uint32_t width, uint32_t height);

  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    std::string m_debugName;
    uint32_t m_viewportWidth, m_viewportHeight;
    
    Ref<FrameBuffer> m_viewportRenderPass;
  };
} // namespace IKan
