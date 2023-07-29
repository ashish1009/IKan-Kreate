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
    
  private:
    // Member Variables
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
