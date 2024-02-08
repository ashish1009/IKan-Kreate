//
//  OpenGLFrameBuffer.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"

namespace IKan
{
  /// Open GL Implementation class to store Framebuffer
  class OpenGLFrameBuffer : public FrameBuffer
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter spec: Framebuffer specificaion
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLFrameBuffer();
    
    /// This function binds the current FrameBuffer to the renderer
    void Bind() const override;
    /// This function unbinds the current FrameBuffer from the renderer
    void Unbind() const override;
    
    /// This function resizes the Framebuffer
    /// - parameters:
    ///   - width: New width of FB
    ///   - height: New height of FB
    void Resize(uint32_t width, uint32_t height) override;
    
    /// This function returns the Frame buffer specification
    const FrameBufferSpecification& GetSpecification() const override;
    /// This function returns the Renderer ID
    RendererID GetRendererId() const override;
    /// This function returns the Depth Attachment ID
    Ref<Texture> GetDepthAttachment() const override;
    /// This function returns the Color Attachment iDs
    const std::vector<Ref<Texture>>& GetColorAttachments() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLFrameBuffer);
    
  private:
    // Member Function -----------------------------------------------------------------------------------------------
    /// This function invalidate the frame buffer on creation on resize
    /// - Parameter debugLogs : Shows the resilze texture logs
    void Invalidate(bool debugLogs = true);
    
    // Member Variables ----------------------------------------------------------------------------------------------
    RendererID m_rendererID{0};
    FrameBufferSpecification m_specification;
    
    // Stores the attachments of Framebuffer
    std::vector<FrameBufferAttachments::TextureFormat> m_colorSpecifications;
    FrameBufferAttachments::TextureFormat m_depthSpecification = FrameBufferAttachments::TextureFormat::None;
    
    // Stores the attachment ID for both color and depth
    std::vector<Ref<Texture>> m_colorAttachments;
    Ref<Texture> m_depthAttachment{0};
  };
} // namespace IKan
