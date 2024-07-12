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

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLRendererAPI);
  };
} // namespace IKan
