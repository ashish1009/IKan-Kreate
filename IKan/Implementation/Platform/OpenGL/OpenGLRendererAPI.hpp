//
//  OpenGLRendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Graphics/RendererAPI.hpp"

namespace IKan
{
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    /// This is the default constructor of Rendere API
    OpenGLRendererAPI();
    /// This is the default destructor ot Renderer API
    virtual ~OpenGLRendererAPI();
    
    /// This function changes the bit value for depth bit
    /// - Parameter state: new depth state
    void Depth(bool state) const override;
    /// This functin changes the bit value for Blend bit
    /// - Parameter state: new Blend state
    void Blend(bool state) const override;
    /// This function changes the bit value of multisample
    /// - Parameter state: new Multi sample state
    void MultiSample(bool state) const override;
    
    /// This function clear all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    void SetClearColor(const glm::vec4& color) const override;
    /// This function clear all the bits of renderer screen
    void ClearBits() const override;
    /// This function Clears Depth Bit
    void ClearDepthBit() const override;
    /// This function Clears Color Biut
    void ClearColorBit() const override;
    /// This function Clears Stensil Bit
    void ClearStencilBit() const override;
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    void SetViewport(uint32_t width, uint32_t height) const override;
    
    /// This funciton returns the Pixel ID from Frame buffer
    /// - Parameters:
    ///   - mx: x pixel
    ///   - my: y pixel
    ///   - pixelIDIndex: pixel index
    ///   - pixelData: pixel value
    void GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixelIDIndex, int32_t& pixelData) const override;
  };
} // namespace IKan
