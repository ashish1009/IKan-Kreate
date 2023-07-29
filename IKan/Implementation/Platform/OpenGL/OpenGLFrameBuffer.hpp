//
//  OpenGLFrameBuffer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Graphics/FrameBuffer.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// Open GL Implementation class to store Framebuffer
  class OpenGLFrameBuffer : public FrameBuffer
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter spec: Framebuffer specificaion
    OpenGLFrameBuffer(const Specification& spec);
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
    
    /// This funciton updates the specification color
    /// - Parameter color: color
    void UpdateSpecificationColor(const glm::vec4& color) override;
    
    /// This function returns the Frame buffer specification
    const Specification& GetSpecification() const override;
    /// This function returns the Renderer ID
    RendererID GetRendererId() const override;
    /// This function returns the Depth Attachment ID
    Ref<Texture> GetDepthAttachment() const override;
    /// This function returns the Color Attachment iDs
    const std::vector<Ref<Texture>>& GetColorAttachments() const override;
    /// This function returns the pixel id (RED_ID) index stored in fremebuffer
    uint32_t GetPixelIdIndex() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLFrameBuffer);
    
  private:
    // Member Function -----------------------------------------------------------------------------------------------
    /// This function invalidate the frame buffer on creation on resize
    /// - Parameter debugLogs : Shows the resilze texture logs
    void Invalidate(bool debugLogs = true);
    
    // Member Variables ----------------------------------------------------------------------------------------------
    RendererID m_rendererID = 0;
    Specification m_specification;
    
    // Stores the attachments of Framebuffer
    std::vector<Attachments::TextureFormat> m_colorSpecifications;
    Attachments::TextureFormat m_depthSpecification = Attachments::TextureFormat::None;
    
    // Stores the attachment ID for both color and depth
    std::vector<Ref<Texture>> m_colorAttachments;
    Ref<Texture> m_depthAttachment = 0;
    
    uint32_t m_pixelIDIndex = 0;
  };
} // namespace IKan
