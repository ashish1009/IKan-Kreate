//
//  OpenGLRendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Graphics/RendererAPI.hpp"

namespace IKan
{
  /// This class implements the Renderer APIs for Open GL.
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    /// This constructor creates the instance of Rendere API.
    OpenGLRendererAPI();
    /// This destructor destroys the Renderer API instance.
    virtual ~OpenGLRendererAPI();
    
    /// This function clear all the renderer bits and set the clear color as 'color'.
    /// - Parameter color: new clear color of screen.
    void SetClearColor(const glm::vec4& color) const override;
    /// This function clear all the bits of renderer screen.
    void ClearBits() const override;
    /// This function clear the color bits of renderer screen.
    void ClearColorBits() const override;
    /// This function clears the depth bits.
    void ClearDepthBits() const override;
    /// This function clears the stenscil bits.
    void ClearStencilBits() const override;

    /// This function update the renderer viewport size.
    /// - Parameters:
    ///   - width: new width of renderer viewport.
    ///   - height: new height of renderer viewport.
    void SetViewport(uint32_t width, uint32_t height) const override;

    /// This API draws a quad with pipeline and indexed count.
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer.
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array).
    void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count) const override;
    /// This API draws Lines Vertex Array.
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer layout.
    ///   - vertexCount: number of Vertices.
    void DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount) const override;

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererAPI);
  };
} // namespace IKan
