//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLTexture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer/RendererStats.hpp"
#include "Platform/OpenGL/OpenGLSpecification.hpp"

namespace IKan
{
#define CHAR_TEXTURE_SUBMIT 0
#define CHAR_DEBUG_TEXTURE_LOG 0
  
  // Open GL Texture ------------------------------------------------------------------------------------
  OpenGLTexture::OpenGLTexture(const Texture2DSpecification& spec)
  : m_specification(spec)
  {
    Renderer::Submit([this]() {
      IK_PROFILE();
      
      // Check the channel of Image data if data is non zero.
      if (m_specification.data)
      {
        m_channel = TextureFormat::RGBA == m_specification.dataFormat ? 4 : 3;
        IK_ASSERT((m_specification.size == m_specification.width * m_specification.height * (uint32_t)m_channel),
                  "Data must be entire texture!");
      }
      
      // Get GL Texture Type.
      GLenum textureType = TextureUtils::OpenGLTypeFromIKanType(m_specification.type);
      
      // Create Texture Buffer.
      glGenTextures(1, &m_rendererID);
      glBindTexture(textureType, m_rendererID);
      
      // Create texture in the renderer Buffer.
      GLenum internalFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.internalFormat);
      GLenum dataFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.dataFormat);
      GLenum dataType = TextureUtils::GetTextureDataType(internalFormat);
      
      if (TextureType::TextureCubemap == m_specification.type)
      {
        for (uint32_t i = 0; i < 6; ++i)
        {
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, (GLint)internalFormat,
                       (GLsizei)m_specification.width, (GLsizei)m_specification.height, 0,
                       dataFormat, dataType, m_specification.data);
          glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
      }
      else
      {
        glTexImage2D((GLenum)textureType, 0, /* Level */ (GLint)internalFormat,
                     (GLsizei)m_specification.width, (GLsizei)m_specification.height, 0 /* Border */,
                     dataFormat, dataType, m_specification.data);
      }
      
      // Set the Texture filter
      glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.minFilter));
      glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.magFilter));
      
      // Set the Texture wraps
      glTexParameteri(textureType, GL_TEXTURE_WRAP_S, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Xwrap));
      glTexParameteri(textureType, GL_TEXTURE_WRAP_T, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Ywrap));
      glTexParameteri(textureType, GL_TEXTURE_WRAP_R, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Zwrap));
      
      // Increment the Stats
      RendererStatistics::Get().textureBufferSize += m_specification.size;
      
      if (m_specification.enableDebugLogs)
      {
        IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Texture 2D ");
        IK_LOG_DEBUG(LogModule::Texture, "  Name              | {0}", m_specification.title);
        IK_LOG_TRACE(LogModule::Texture, "  Renderer ID       | {0}", m_rendererID);
        IK_LOG_TRACE(LogModule::Texture, "  Size              | {0} B", m_specification.size);
        IK_LOG_TRACE(LogModule::Texture, "  Channels          | {0}", m_channel);
        IK_LOG_TRACE(LogModule::Texture, "  Internal Format   | {0}", TextureUtils::IKanFormatName(m_specification.internalFormat));
        IK_LOG_TRACE(LogModule::Texture, "  Data Format       | {0}", TextureUtils::IKanFormatName(m_specification.dataFormat));
        IK_LOG_TRACE(LogModule::Texture, "  Type              | {0}", TextureUtils::IKanTypeName(m_specification.type));
        IK_LOG_TRACE(LogModule::Texture, "  X Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Xwrap));
        IK_LOG_TRACE(LogModule::Texture, "  Y Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Ywrap));
        IK_LOG_TRACE(LogModule::Texture, "  Z Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Zwrap));
        IK_LOG_TRACE(LogModule::Texture, "  Min Filter        | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
        IK_LOG_TRACE(LogModule::Texture, "  mag Filter        | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
        IK_LOG_TRACE(LogModule::Texture, "  Total Size in GPU | {0} B ({1} KB, {2} MB)",
                     RendererStatistics::Get().textureBufferSize,
                     RendererStatistics::Get().textureBufferSize / 1000,
                     RendererStatistics::Get().textureBufferSize / 1000000);
      }
    });
  }
  OpenGLTexture::~OpenGLTexture()
  {
    IK_PROFILE();
    if (m_specification.data)
    {
      RendererStatistics::Get().textureBufferSize -= m_specification.size;
    }
    
    if (m_specification.enableDebugLogs)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Texture 2D ");
      IK_LOG_DEBUG(LogModule::Texture, "  Name              | {0}", m_specification.title);
      IK_LOG_TRACE(LogModule::Texture, "  Renderer ID       | {0}", m_rendererID);
      IK_LOG_TRACE(LogModule::Texture, "  Size              | {0} B", m_specification.size);
      IK_LOG_TRACE(LogModule::Texture, "  Channels          | {0}", m_channel);
      IK_LOG_TRACE(LogModule::Texture, "  Internal Format   | {0}", TextureUtils::IKanFormatName(m_specification.internalFormat));
      IK_LOG_TRACE(LogModule::Texture, "  Data Format       | {0}", TextureUtils::IKanFormatName(m_specification.dataFormat));
      IK_LOG_TRACE(LogModule::Texture, "  Type              | {0}", TextureUtils::IKanTypeName(m_specification.type));
      IK_LOG_TRACE(LogModule::Texture, "  X Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Xwrap));
      IK_LOG_TRACE(LogModule::Texture, "  Y Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Ywrap));
      IK_LOG_TRACE(LogModule::Texture, "  Z Wrap            | {0}", TextureUtils::IKanWrapName(m_specification.Zwrap));
      IK_LOG_TRACE(LogModule::Texture, "  Min Filter        | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
      IK_LOG_TRACE(LogModule::Texture, "  mag Filter        | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
      IK_LOG_TRACE(LogModule::Texture, "  Total Size in GPU | {0} B ({1} KB, {2} MB)",
                   RendererStatistics::Get().textureBufferSize,
                   RendererStatistics::Get().textureBufferSize / 1000,
                   RendererStatistics::Get().textureBufferSize / 1000000);
    }
    Renderer::Submit([this] {
      glDeleteTextures(1, &m_rendererID);
    });
  }
  
  void OpenGLTexture::Bind(uint32_t slot) const
  {
    Renderer::Submit([this, slot]() {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(TextureUtils::OpenGLTypeFromIKanType(m_specification.type), m_rendererID);
    });
  }
  
  void OpenGLTexture::Unbind() const
  {
    Renderer::Submit([]() {
      glBindTexture(GL_TEXTURE_2D, 0);
    });
  }
  
  void OpenGLTexture::AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID, uint32_t depthID, uint32_t level) const
  {
    Renderer::Submit([this, attachmentType, colorID, depthID, level](){
      TextureUtils::AttachTexture(m_rendererID, m_specification.type, m_specification.internalFormat, attachmentType, colorID, depthID, level);
    });
  }
  
  RendererID OpenGLTexture::GetRendererID() const
  {
    return m_rendererID;
  }
  uint32_t OpenGLTexture::GetWidth() const
  {
    return m_specification.width;
  }
  uint32_t OpenGLTexture::GetHeight() const
  {
    return m_specification.height;
  }
  
  // Open GL Image ------------------------------------------------------------------------------------
  OpenGLImage::OpenGLImage(const ImageSpecificaion& spec)
  : m_specification(spec)
  {
    IK_PROFILE();
    m_name = m_specification.filePath.filename();
    
    Renderer::Submit([this]() {
      // Invert Image
      if (m_specification.invertVertically)
      {
        stbi_set_flip_vertically_on_load(1);
      }
      
      // Load the file with stb image API
      // TODO: IKan: Move this outside Submit
      void* data = stbi_load(m_specification.filePath.c_str(), &m_width, &m_height, &m_channel, 0 /* desired_channels */);
      
      IK_ASSERT(data, "Data is NULL!");
      
      // If file loaded successfullY
      TextureFormat internalFormat = TextureFormat::RGBA8;
      TextureFormat dataFormat = TextureFormat::RGBA;
      
      switch (m_channel)
      {
        case 4 :
          internalFormat = TextureUtils::OpenGLFormatToIKanFormat(GL_RGBA8);
          dataFormat     = TextureUtils::OpenGLFormatToIKanFormat(GL_RGBA);
          break;
        case 3 :
          internalFormat = TextureUtils::OpenGLFormatToIKanFormat(GL_RGB8);
          dataFormat     = TextureUtils::OpenGLFormatToIKanFormat(GL_RGB);
          break;
        case 2 :
        case 1 :
          internalFormat = TextureUtils::OpenGLFormatToIKanFormat(GL_RED);
          dataFormat     = TextureUtils::OpenGLFormatToIKanFormat(GL_RED);
          break;
          
        default:
          IK_ASSERT(false, "Invalid Format!");
      }
      
      glGenTextures(1, &m_rendererID);
      glBindTexture(GL_TEXTURE_2D, m_rendererID);
      
      // Set the Texture filter
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.minFilter));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.magFilter));
      
      // Set the Texture wraps
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Xwrap));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Ywrap));
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, TextureUtils::OpenGLWrapFromIKanWrap(m_specification.Zwrap));
      
      // Create texture in the renderer Buffer
      GLenum glInternalFormat = TextureUtils::OpenGLFormatFromIKanFormat(internalFormat);
      GLenum glDataFormat = TextureUtils::OpenGLFormatFromIKanFormat(dataFormat);
      
      glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ (GLint)glInternalFormat, (GLsizei)m_width, (GLsizei)m_height, 0,
                   /* Border */ glDataFormat, TextureUtils::GetTextureDataType(glInternalFormat), data);
      
      // Store the size of texture in Data
      m_size = (uint32_t)m_width * (uint32_t)m_height * (uint32_t)m_channel;
      
      // Increment the size in stats
      RendererStatistics::Get().textureBufferSize += m_size;
      
      // Delete the data as we have already loaded in graphics
      delete (stbi_uc*)data;
      
      IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Image Texture from File ... ");
      IK_LOG_DEBUG(LogModule::Texture, "  File Path          | {0}", Utils::FileSystem::IKanAbsolute(m_specification.filePath).string());
      IK_LOG_TRACE(LogModule::Texture, "  Renderer ID        | {0}", m_rendererID);
      IK_LOG_TRACE(LogModule::Texture, "  Width              | {0}", m_width);
      IK_LOG_TRACE(LogModule::Texture, "  Height             | {0}", m_height);
      IK_LOG_TRACE(LogModule::Texture, "  Size               | {0} B", m_size);
      IK_LOG_TRACE(LogModule::Texture, "  Number of Channel  | {0}", m_channel);
      IK_LOG_TRACE(LogModule::Texture, "  Internal Format    | {0}", TextureUtils::IKanFormatName(internalFormat));
      IK_LOG_TRACE(LogModule::Texture, "  Data Format        | {0}", TextureUtils::IKanFormatName(dataFormat));
      IK_LOG_TRACE(LogModule::Texture, "  X Wrap             | {0}", TextureUtils::IKanWrapName(m_specification.Xwrap));
      IK_LOG_TRACE(LogModule::Texture, "  Y Wrap             | {0}", TextureUtils::IKanWrapName(m_specification.Ywrap));
      IK_LOG_TRACE(LogModule::Texture, "  Z Wrap             | {0}", TextureUtils::IKanWrapName(m_specification.Zwrap));
      IK_LOG_TRACE(LogModule::Texture, "  Min Filter         | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
      IK_LOG_TRACE(LogModule::Texture, "  mag Filter         | {0}", TextureUtils::IKanFilterName(m_specification.minFilter));
      IK_LOG_TRACE(LogModule::Texture, "  Total Size in GPU  | {0} B ({1} KB, {2} MB",
                   RendererStatistics::Get().textureBufferSize,
                   RendererStatistics::Get().textureBufferSize / 1000,
                   RendererStatistics::Get().textureBufferSize / 1000000);
      
    });
  }
  
  OpenGLImage::~OpenGLImage()
  {
    IK_PROFILE();
    
    IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Image Texture ");
    IK_LOG_DEBUG(LogModule::Texture, "  File Path         | {0}", Utils::FileSystem::IKanAbsolute(m_specification.filePath).string());
    IK_LOG_TRACE(LogModule::Texture, "  Renderer ID       | {0}", m_rendererID);
    
    RendererStatistics::Get().textureBufferSize -= m_size;
    
    Renderer::Submit([this] {
      glDeleteTextures(1, &m_rendererID);
    });
  }
  
  void OpenGLImage::Bind(uint32_t slot) const
  {
    Renderer::Submit([this, slot]() {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_rendererID);
    });
  }
  void OpenGLImage::Unbind() const
  {
    Renderer::Submit([]() {
      glBindTexture(GL_TEXTURE_2D, 0);
    });
  }
  
  void OpenGLImage::AttachToFramebuffer([[maybe_unused]] TextureAttachment attachmentType, [[maybe_unused]] uint32_t colorID,
                                        [[maybe_unused]] uint32_t depthID, [[maybe_unused]] uint32_t level) const
  {
    IK_ASSERT(false);
  }

  RendererID OpenGLImage::GetRendererID() const
  {
    return m_rendererID;
  }
  uint32_t OpenGLImage::GetWidth() const
  {
    return (uint32_t)m_width;
  }
  uint32_t OpenGLImage::GetHeight() const
  {
    return (uint32_t)m_height;
  }
  
  const std::filesystem::path& OpenGLImage::GetfilePath() const
  {
    return m_specification.filePath;
  }
  
  const std::string& OpenGLImage::GetName() const
  {
    return m_name;
  }
  
  // Open GL Char Texture -------------------------------------------------------------------------------
  OpenGLCharTexture::OpenGLCharTexture(const CharTextureSpecification& charTextureSpec)
  : m_specificaion(charTextureSpec)
  {
    // Note: Do not submite in queue as this face pointer is local is not copied. This happens once so no runtime issues
#if CHAR_TEXTURE_SUBMIT
    Renderer::Submit([this, name, values, count]() {
#endif
      IK_PROFILE();
      glGenTextures(1, &m_rendererID);
      glBindTexture(GL_TEXTURE_2D, m_rendererID);
      
      // Create texture in the renderer Buffer
      glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ GL_RED, (GLsizei)m_specificaion.face->glyph->bitmap.width, (GLsizei)m_specificaion.face->glyph->bitmap.rows,
                   0, /* Border */ GL_RED, GL_UNSIGNED_BYTE, m_specificaion.face->glyph->bitmap.buffer );
      
      m_width = (uint32_t)m_specificaion.face->glyph->bitmap.width;
      m_height = (uint32_t)m_specificaion.face->glyph->bitmap.rows;
      
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      
#if CHAR_DEBUG_TEXTURE_LOG
      IK_LOG_TRACE(LogModule::CharTexture, "Creating Open GL Char Texture to store Char {0} ( Renderer ID {1} )", m_specificaion.charVal, m_rendererID);
#endif
      
      // Increment the size in stats
      m_dataSize = m_specificaion.face->glyph->bitmap.width * m_specificaion.face->glyph->bitmap.rows;
      RendererStatistics::Get().textureBufferSize += m_dataSize;
      
#if CHAR_TEXTURE_SUBMIT
    });
#endif
  }
  
  OpenGLCharTexture::~OpenGLCharTexture()
  {
    IK_PROFILE();
    RendererStatistics::Get().textureBufferSize -= m_dataSize;
#if CHAR_TEXTURE_SUBMIT
    Renderer::Submit([this] {
#endif
      glDeleteTextures(1, &m_rendererID);
#if CHAR_TEXTURE_SUBMIT
    });
#endif
  }
  
  void OpenGLCharTexture::Bind(uint32_t slot) const
  {
    Renderer::Submit([this, slot] {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_rendererID);
    });
  }
  void OpenGLCharTexture::Unbind() const
  {
    Renderer::Submit([] {
      glBindTexture(GL_TEXTURE_2D, 0);
    });
  }
  
  void OpenGLCharTexture::AttachToFramebuffer([[maybe_unused]] TextureAttachment attachmentType, [[maybe_unused]] uint32_t colorID,
                                              [[maybe_unused]] uint32_t depthID, [[maybe_unused]] uint32_t level) const
  {
    IK_ASSERT(false);
  }
  
  RendererID OpenGLCharTexture::GetRendererID() const
  {
    return m_rendererID;
  }
  uint32_t OpenGLCharTexture::GetWidth() const
  {
    return m_width;
  }
  uint32_t OpenGLCharTexture::GetHeight() const
  {
    return m_height;
  }
  glm::ivec2 OpenGLCharTexture::GetSize() const
  {
    return m_specificaion.size;
  }
  glm::ivec2 OpenGLCharTexture::GetBearing() const
  {
    return m_specificaion.bearing;
  }
  uint32_t OpenGLCharTexture::GetAdvance() const
  {
    return m_specificaion.advance;
  }
  
} // namespace IKan
