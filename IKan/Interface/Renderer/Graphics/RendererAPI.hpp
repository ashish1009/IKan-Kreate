//
//  RendererAPI.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class is the interface for handling all the renderer APIs
  class RendererAPI
  {
  public:
    /// This is the default virtual destrutctor
    virtual ~RendererAPI() = default;
    
    /// This function clears all the renderer bits and set the clear color as 'color'
    /// - Parameter color: new clear color of screen
    virtual void SetClearColor(const glm::vec4& color) const = 0;
    /// This function clears all the bits of renderer screen
    virtual void ClearBits() const = 0;
    /// This function clears the color bits
    virtual void ClearColorBits() const = 0;
    /// This function clears the depth bits
    virtual void ClearDepthBits() const = 0;
    /// This function clears the depth bits
    virtual void ClearStencilBits() const = 0;
  };
} // namespace IKan
