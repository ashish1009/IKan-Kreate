//
//  OpenGLFrameBuffer.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include <glad/glad.h>
#include "OpenGLFrameBuffer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace IKan
{
#define FB_LOG(...) IK_LOG_DEBUG(LogModule::FrameBuffer, __VA_ARGS__);
#define GET_FORMAT_NAME(name) TextureUtils::IKanFormatName(name)
  
  static GLint s_rendererViewport[4];
  
  namespace FramebufferUtils
  {
    /// This function checks is the specification format is depth or not
    /// - Parameter format: Format type
    static bool IsDepthFormat(FrameBufferAttachments::TextureFormat format)
    {
      switch (format)
      {
        case FrameBufferAttachments::TextureFormat::RGBA8: return false;
        case FrameBufferAttachments::TextureFormat::RGBA16F: return false;
        case FrameBufferAttachments::TextureFormat::R32I: return false;
        case FrameBufferAttachments::TextureFormat::RED: return false;
        case FrameBufferAttachments::TextureFormat::Depth24Stencil: return true;
        case FrameBufferAttachments::TextureFormat::DepthCubeMap: return true;
        case FrameBufferAttachments::TextureFormat::None:
        default:
          IK_ASSERT(false, "invalid format");
      }
    }
  } // namespace FramebufferUtils
  
  OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
  : m_specification(spec)
  {
    IK_PROFILE();
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
    IK_PROFILE();
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
    IK_PROFILE();
    if (debugLogs)
    {
      FB_LOG("Invalidating Open GL Framebuffer : {0}", m_specification.debugName);
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
    
    Texture2DSpecification spec;
    spec.width  = m_specification.width;
    spec.height = m_specification.height;
    
    // Color Attachments
    if (m_colorSpecifications.size())
    {
      m_colorAttachments.resize(m_colorSpecifications.size());
      
      // For each color attachment
      for (size_t i = 0; i < m_colorSpecifications.size(); i++)
      {
        switch (m_colorSpecifications[i])
        {
          case FrameBufferAttachments::TextureFormat::None:
          case FrameBufferAttachments::TextureFormat::Depth24Stencil:
          case FrameBufferAttachments::TextureFormat::DepthCubeMap:
            break;
            
          case FrameBufferAttachments::TextureFormat::RGBA8:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::RGBA8;
            spec.dataFormat       = TextureFormat::RGBA;
            m_colorAttachments[i] = TextureFactory::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            break;
          }
          case FrameBufferAttachments::TextureFormat::RGBA16F:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::RGBA16F;
            spec.dataFormat       = TextureFormat::RGBA;
            spec.wrap             = TextureWrap::ClampEdge;
            m_colorAttachments[i] = TextureFactory::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            
            break;
          }
          case FrameBufferAttachments::TextureFormat::R32I:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::R32I;
            spec.dataFormat       = TextureFormat::RED_INTEGER;
            m_colorAttachments[i] = TextureFactory::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            break;
          }
          case FrameBufferAttachments::TextureFormat::RED:
          {
            // Create the Color Image from the specification
            spec.internalFormat   = TextureFormat::RED;
            spec.dataFormat       = TextureFormat::RED;
            m_colorAttachments[i] = TextureFactory::Create(spec);
            
            // Attach Image to Framebuffer
            m_colorAttachments[i]->AttachToFramebuffer(TextureAttachment::Color, (uint32_t)i);
            break;
          }
        } // swtch(color attachment)
        FB_LOG("  Color Attachment : {0} | {1}", GET_FORMAT_NAME(spec.internalFormat), GET_FORMAT_NAME(spec.dataFormat));
      }
    } // Color Attachments
    
    // Depth Attachment
    if (m_depthSpecification != FrameBufferAttachments::TextureFormat::None)
    {
      switch (m_depthSpecification)
      {
        case FrameBufferAttachments::TextureFormat::None:
        case FrameBufferAttachments::TextureFormat::RGBA8:
        case FrameBufferAttachments::TextureFormat::R32I:
        case FrameBufferAttachments::TextureFormat::RGBA16F:
        case FrameBufferAttachments::TextureFormat::RED:
          break;
          
        case FrameBufferAttachments::TextureFormat::Depth24Stencil:
        {
          // Create the Depth Image from the specification
          spec.internalFormat = TextureFormat::DEPTH_COMPONENT;
          spec.dataFormat     = TextureFormat::DEPTH_COMPONENT;
          spec.wrap           = TextureWrap::ClampBorder;
          spec.filter         = TextureFilter::Nearest;
          m_depthAttachment   = TextureFactory::Create(spec);
          
          // Attach Image to Framebuffer
          m_depthAttachment->AttachToFramebuffer(TextureAttachment::Depth);
          break;
        }
        case FrameBufferAttachments::TextureFormat::DepthCubeMap:
        {
          spec.internalFormat = TextureFormat::DEPTH_COMPONENT;
          spec.dataFormat     = TextureFormat::DEPTH_COMPONENT;
          spec.type           = TextureType::TextureCubemap;
          spec.wrap           = TextureWrap::ClampEdge;
          spec.filter         = TextureFilter::Nearest;
          m_depthAttachment   = TextureFactory::Create(spec);
          
          // Attach Image to Framebuffer
          m_depthAttachment->AttachToFramebuffer(TextureAttachment::Depth);
          break;
        }
      }
      FB_LOG("  Depth Attachment : {0} | {1}", GET_FORMAT_NAME(spec.internalFormat), GET_FORMAT_NAME(spec.dataFormat));
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
    else if (m_colorSpecifications.empty() and m_depthSpecification != FrameBufferAttachments::TextureFormat::None)
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
  
  void OpenGLFrameBuffer::Bind() const
  {
    glGetIntegerv(GL_VIEWPORT, s_rendererViewport);
    glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
    Renderer::SetViewport(m_specification.width, m_specification.height);
  }
  
  void OpenGLFrameBuffer::Unbind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(s_rendererViewport[0], s_rendererViewport[1], (GLsizei)s_rendererViewport[2], (GLsizei)s_rendererViewport[3]);
  }
  
  void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
  {
    RETURN_IF(m_specification.width == width and m_specification.height == height);
    
    glGetIntegerv(GL_VIEWPORT, s_rendererViewport);
    
    m_specification.width  = width;
    m_specification.height = height;
    
    FB_LOG("Resizing the Framebuffer  : {0}", m_specification.debugName);
    FB_LOG("  Size             : {0} x {1}", m_specification.width, m_specification.height);
    Invalidate(false);
    
    Renderer::SetViewport(m_specification.width, m_specification.height);
    glViewport(s_rendererViewport[0], s_rendererViewport[1], (GLsizei)s_rendererViewport[2], (GLsizei)s_rendererViewport[3]);
  }
  
  const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const
  {
    return m_specification;
  }
  
  RendererID OpenGLFrameBuffer::GetRendererId() const
  {
    return m_rendererID;
  }
  
  Ref<Texture> OpenGLFrameBuffer::GetDepthAttachment() const
  {
    return m_depthAttachment;
  }
  
  const std::vector<Ref<Texture>>& OpenGLFrameBuffer::GetColorAttachments() const
  {
    return m_colorAttachments;
  }
} // namespace IKan
