//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class is the interface for creating texture with specification data
  class Texture 
  {
  public:
    /// Default virtual destructor
    virtual ~Texture() = default;
    
    /// This function binds the current Texture to the renderer
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the current Texture from the renderer
    virtual void Unbind() const = 0;
    
    /// This function attaches the current Image to Framebuffer
    /// - Parameters:
    ///   - colorID : Color Attaachment ID to Framebuffer
    ///   - depthID : Depth Attachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    virtual void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const = 0;
    
    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;
  };
} //namespace IKan
