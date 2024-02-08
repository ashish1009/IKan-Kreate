//
//  OpenGLRendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/RendererAPI.hpp"

namespace IKan
{
  /// This is the implementation class for Renderer API
  class OpenGLRendererAPI : public RendererAPI
  {
  public:
    /// This is the default constructor of Rendere API
    OpenGLRendererAPI();
    /// This is the default destructor ot Renderer API
    virtual ~OpenGLRendererAPI();
    
    /// This function clear all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    void SetClearColor(const glm::vec4& color) const override;
    /// This function clear all the bits of renderer screen
    void ClearBits() const override;
    /// This function clears the depth bits
    void ClearDepthBits() const override;
    /// This function clears the depth bits
    void ClearStencilBits() const override;

    /// This function enables the depth pass
    void EnableDepthPass() const override;
    /// This function disables the depth pass
    void DisableDepthPass() const override;
    /// This function change depth function
    /// - Parameter func: depth function type
    void DepthFunc(GlDepthFunc func) const override;
    
    /// This function enables the stencil pass
    void EnableStencilPass() const override;
    /// This function disables the stencil pass
    void DisableStencilPass() const override;
    
    /// This function update the renderer viewport size
    /// - Parameters:
    ///   - width: new width of renderer viewport
    ///   - height: new height of renderer viewport
    void SetViewport(uint32_t width, uint32_t height) const override;
  };
} // namespace IKan
