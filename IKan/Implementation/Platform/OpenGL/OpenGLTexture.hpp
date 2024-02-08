//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// Interface class for storing Texture
  class OpenGLTexture : public Texture
  {
  public:
    /// Default Open GL Image Constructor
    /// - Parameter spec: Open GL Image Specification
    OpenGLTexture(const Texture2DSpecification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLTexture();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the current Texture from the renderer
    void Unbind() const override;
    
    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override;
    
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - colorID : Color Attaachment ID to Framebuffer
    ///   - depthID : Depth Attachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLTexture);
    
  private:
    RendererID m_rendererID {0};
    Texture2DSpecification m_specification;
    int32_t m_channel {0};
  };

} // namespace IKan
