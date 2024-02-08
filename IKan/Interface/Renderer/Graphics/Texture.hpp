//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.h"

namespace IKan
{
  /// Interface class for storing Texture with specificaion data
  class Texture
  {
  public:
    /// Default virtual destructor
    virtual ~Texture() = default;
    
    /// This function binds the current Texture to the renderer
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the current Texture from the renderer
    virtual void Unbind() const = 0;
    
    /// This function attach the current Image to Framebuffer
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

  /// This is the interface class to create the Texture 2D for Framebuffers
  class Texture2D : public Texture
  {
  public:
    /// This is default virtual destructor for Texture
    virtual ~Texture2D() = default;
  };
  
  

} // namespace IKan
