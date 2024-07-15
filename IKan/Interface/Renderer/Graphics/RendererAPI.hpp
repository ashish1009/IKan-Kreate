//
//  RendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.hpp"

namespace IKan
{
  /// This class is the interface for all the renderer APIs
  class RendererAPI
  {
  public:
    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
    
    /// This function clear all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    virtual void SetClearColor(const glm::vec4& color) const = 0;
    /// This function clear all the bits of renderer screen
    virtual void ClearBits() const = 0;
    /// This function clears the color bits
    virtual void ClearColorBits() const = 0;
    /// This function clears the depth bits
    virtual void ClearDepthBits() const = 0;
    /// This function clears the depth bits
    virtual void ClearStencilBits() const = 0;
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    virtual void SetViewport(uint32_t width, uint32_t height) const = 0;
    
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count) const = 0;
    /// This API draws Lines Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer layout
    ///   - vertexCount: number of vertices
    virtual void DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount) const = 0;
    /// This API draws Indexed Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer attribute
    ///   - count: number of vertices
    virtual void DrawArrays(const Ref<Pipeline>& pipeline, uint32_t count) const = 0;
    /// This API draws Quad
    /// - Parameter pipeline: pipeline having vertex buffer and index buffer
    virtual void DrawQuad(const Ref<Pipeline>& pipeline) const = 0;
    /// This function draw the index based vertex
    /// - Parameters:
    ///   - indexCount: index count
    ///   - indicesData: indices data
    ///   - baseVertex: base vertex
    virtual void DrawIndexedBaseVertex(uint32_t indexCount, void* indicesData, uint32_t baseVertex) const = 0;
  };
} // namespace IKan
