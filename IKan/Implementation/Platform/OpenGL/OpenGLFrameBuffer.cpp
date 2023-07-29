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
    
    Invalidate();
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
  
  void OpenGLFrameBuffer::Invalidate(bool debugLogs)
  {
    if (debugLogs)
    {
      FB_LOG("Invalidating Open GL Framebuffer");
    }
    
    // Delete the framebuffers if already created
    if (m_rendererID)
    {
      m_depthAttachment.reset();
      for (auto& ca : m_colorAttachments)
      {
        ca.reset();
      }
      glDeleteFramebuffers(1, &m_rendererID);
      
      m_colorAttachments.clear();
    }
    
    // Generate the frame buffer to renderer ID
    glGenFramebuffers(1, &m_rendererID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        
    Texture::Specification spec;
    spec.width  = m_specification.width;
    spec.height = m_specification.height;
    
    // Color Attachments
    if (m_colorSpecifications.size())
    {
      m_colorAttachments.resize(m_colorSpecifications.size());
      
      // For each color attachment
      for (size_t i = 0; i < m_colorAttachments.size(); i++)
      {
        switch (m_colorSpecifications[i])
        {
          case FrameBuffer::Attachments::TextureFormat::None:
          case FrameBuffer::Attachments::TextureFormat::Depth24Stencil:
            break;
            
          case FrameBuffer::Attachments::TextureFormat::RGBA8:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::RGBA8;
            spec.dataFormat       = TextureFormat::RGBA;
            m_colorAttachments[i] = Texture::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            break;
          }
          case FrameBuffer::Attachments::TextureFormat::R32I:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::R32I;
            spec.dataFormat       = TextureFormat::RED_INTEGER;
            m_colorAttachments[i] = Texture::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            
            // Stores the Index of Color attachment used to store the Pixel ID
            m_pixelIDIndex = (uint32_t)i;
            break;
          }
        }
      }
    } // Color Attachments
    
    // Depth Attachment
    if (m_depthSpecification != FrameBuffer::Attachments::TextureFormat::None)
    {
      switch (m_depthSpecification)
      {
        case FrameBuffer::Attachments::TextureFormat::None:
        case FrameBuffer::Attachments::TextureFormat::RGBA8:
        case FrameBuffer::Attachments::TextureFormat::R32I:
          break;
          
        case FrameBuffer::Attachments::TextureFormat::Depth24Stencil:
        {
          // Create the Depth Image from the specification
          spec.internalFormat = TextureFormat::DEPTH_COMPONENT;
          spec.dataFormat     = TextureFormat::DEPTH_COMPONENT;
          m_depthAttachment   = Texture::Create(spec);
          
          // Attach Image to Framebuffer
          m_depthAttachment->AttachToFramebuffer(TextureAttachment::Depth);
          break;
        }
      };
    } // Depth Attachment
    
    // Error check
    if (m_colorAttachments.size() >= 1)
    {
      IK_ASSERT((m_colorAttachments.size() <= 4), "Inalid Attachment");
      static GLenum buffers[4] =
      {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2,
        GL_COLOR_ATTACHMENT3
      };
      
      glDrawBuffers((GLsizei)m_colorAttachments.size(), buffers);
      IK_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "FrameBuffer is Incomplete ");
    }
    else if (m_colorSpecifications.empty() and m_depthSpecification != FrameBuffer::Attachments::TextureFormat::None)
    {
      // Only depth-pass
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
      
      static float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
      
      IK_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE), "FrameBuffer is Incomplete ");
    }
    else
    {
      FB_LOG("Framebuffer created without attachment");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
} // namespace IKan
