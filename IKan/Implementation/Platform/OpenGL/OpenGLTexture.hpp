//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// This class implements the APIs to create texture for Open GL.
  class OpenGLTexture : public Texture
  {
  public:
    /// This constructor creates the Open GL texture from specification data.
    /// - Parameter spec: Open GL Texture specification data.
    OpenGLTexture(const Texture2DSpecification& spec);
    /// This destructor destroyes and deletes the Open GL texture.
    ~OpenGLTexture();
    
    /// This function binds the texture at slot inside shader.
    /// - Parameter slot: shader slot where this texture to be binded.
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the current texture from the renderer.
    void Unbind() const override;
    
    /// This function attaches the current Image to Framebuffer.
    /// - Parameters:
    ///   - attachmentType: attachment type of texture.
    ///   - colorID : Color Attaachment ID to Framebuffer.
    ///   - depthID : Depth Attachment ID to Framebuffer.
    ///   - level : level of framebuffer attachment.
    void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const override;
    
    /// This function returns width of texture.
    uint32_t GetWidth() const override;
    /// This function returns height of texture.
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture.
    RendererID GetRendererID() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLTexture);

  private:
    RendererID m_rendererID {0};
    Texture2DSpecification m_specification;
    int32_t m_channel {0};
  };
} // namespace IKan
