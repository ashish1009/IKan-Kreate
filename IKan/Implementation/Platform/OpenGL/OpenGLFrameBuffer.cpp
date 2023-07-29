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
#define GET_FORMAT_NAME(name) TextureUtils::IKanFormatName(name)

  static GLint s_rendererViewport[4];

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

#ifdef IK_ENABLE_LOG
    // Store the vertex attribute
    Table::Data tableData;
    tableData.title = "Framebuffer Attachments with Renderer ID " + std::to_string(m_rendererID);
    tableData.numColumns = 3;
    tableData.columnHeaders =
    {
      "Internal Format", "Data Format", "ID"
    };
    Table fbTable(tableData);
#endif
    
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

#ifdef IK_ENABLE_LOG
            const RendererID& id = m_colorAttachments[i]->GetRendererID();
            fbTable.AddRow({GET_FORMAT_NAME(spec.internalFormat), GET_FORMAT_NAME(spec.dataFormat), std::to_string(id)});
#endif
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
            
#ifdef IK_ENABLE_LOG
            const RendererID& id = m_colorAttachments[i]->GetRendererID();
            fbTable.AddRow({GET_FORMAT_NAME(spec.internalFormat), GET_FORMAT_NAME(spec.dataFormat), std::to_string(id)});
#endif

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
          
#ifdef IK_ENABLE_LOG
          const RendererID& id = m_depthAttachment->GetRendererID();
          fbTable.AddRow({GET_FORMAT_NAME(spec.internalFormat), GET_FORMAT_NAME(spec.dataFormat), std::to_string(id)});
#endif
          break;
        }
      };
    } // Depth Attachment
    
#ifdef IK_ENABLE_LOG
    // Log the Framebuffer attachment if there is any
    if (debugLogs and
        (m_colorAttachments.size() > 0 or m_depthSpecification != FrameBuffer::Attachments::TextureFormat::None))
    {
      fbTable.Dump(Log::Level::Debug, Log::GetModuleName(LogModule::FrameBuffer));
    }
#endif

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
    glGetIntegerv(GL_VIEWPORT, s_rendererViewport);
    
    m_specification.width  = width;
    m_specification.height = height;
    
    FB_LOG("Resizing the Framebuffer {0} x {1}", m_specification.width, m_specification.height);
    Invalidate(false);
    
    Renderer::SetViewport(m_specification.width, m_specification.height);
    glViewport(s_rendererViewport[0], s_rendererViewport[1], (GLsizei)s_rendererViewport[2], (GLsizei)s_rendererViewport[3]);
  }
  
  void OpenGLFrameBuffer::UpdateSpecificationColor(const glm::vec4& color)
  {
    m_specification.color = color;
  }
  
  const FrameBuffer::Specification& OpenGLFrameBuffer::GetSpecification() const
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
  
  uint32_t OpenGLFrameBuffer::GetPixelIdIndex() const
  {
    return m_pixelIDIndex;
  }

} // namespace IKan
