//
//  OpenGLFrameBuffer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include <glad/glad.h>
#include "OpenGLFrameBuffer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace IKan
{
#define FB_LOG(...) IK_LOG_DEBUG(LogModule::FrameBuffer, __VA_ARGS__);

  namespace FramebufferUtils
  {
    /// This function checks is the specification format is depth or not
    /// - Parameter format: Format type
    static bool IsDepthFormat(FrameBuffer::Attachments::TextureFormat format)
    {
      switch (format)
      {
        case FrameBuffer::Attachments::TextureFormat::RGBA8: return false;
        case FrameBuffer::Attachments::TextureFormat::R32I: return false;
        case FrameBuffer::Attachments::TextureFormat::Depth24Stencil: return true;
        case FrameBuffer::Attachments::TextureFormat::None:
        default:
          IK_ASSERT(false, "invalid format");
      }
    }
  } // namespace FramebufferUtils

  OpenGLFrameBuffer::OpenGLFrameBuffer(const Specification& spec)
  : m_specification(spec)
  {
    FB_LOG("Creating Open GL Framebuffer");
    
    for (auto attchmentFormat : m_specification.attachments.textureFormats)
    {
      if (FramebufferUtils::IsDepthFormat(attchmentFormat))
      {
        m_depthSpecification = attchmentFormat;
      }
      else
      {
        m_colorSpecifications.emplace_back(attchmentFormat);
      }
    }
  }
  
  OpenGLFrameBuffer::~OpenGLFrameBuffer()
  {
    FB_LOG("Destroying Open GL Framebuffer with Renderer ID {0}", m_rendererID);

    m_depthAttachment.reset();
    for (auto& ca : m_colorAttachments)
    {
      ca.reset();
    }
    glDeleteFramebuffers(1, &m_rendererID);
  }
} // namespace IKan
