//
//  RendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

namespace IKan
{
  class Pipeline;

  /// This is the interface class for all Renderer APIs that are Renderer Specific
  class RendererAPI
  {
  public:
    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
    
    /// This function changes the bit value for depth bit
    /// - Parameter state: new depth state
    virtual void Depth(bool state) const = 0;
    /// This functin changes the bit value for Blend bit
    /// - Parameter state: new Blend state
    virtual void Blend(bool state) const = 0;
    /// This function changes the bit value of multisample
    /// - Parameter state: new Multi sample state
    virtual void MultiSample(bool state) const = 0;
    
    /// This function clear all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    virtual void SetClearColor(const glm::vec4& color) const = 0;
    /// This function clear all the bits of renderer screen
    virtual void ClearBits() const = 0;
    /// This function Clears Depth Bit
    virtual void ClearDepthBit() const = 0;
    /// This function Clears Color Biut
    virtual void ClearColorBit() const = 0;
    /// This function Clears Stensil Bit
    virtual void ClearStencilBit() const = 0;
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    virtual void SetViewport(uint32_t width, uint32_t height) const = 0;

    /// This funciton returns the Pixel ID from Frame buffer
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixelIDIndex: pixel index
    ///   - pixelData: pixel value
    virtual void GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixelIDIndex, int32_t& pixelData) const = 0;
    
    /// This API draws a quad with pipeline and indexed count
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count) const = 0;
    /// This API draws Lines Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - vertexCount: number of Indices (if 0 then use index buffer of Vertex array)
    virtual void DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount) const = 0;
    /// This API draws Indexed Vertex Array
    /// - Parameters:
    ///   - pipeline: pipeline having vertex buffer and index buffer
    ///   - count: number of Indices (if 0 then use index buffer of Vertex array)
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

    /// This function creates the Renderer API instance based on the Supported APIs
    [[nodiscard]] static Scope<RendererAPI> Create();
  };
} // namespace IKan
