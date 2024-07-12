//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLTexture.hpp"

#include <glad/glad.h>

#include "Renderer/RendererStats.hpp"
#include "Platform/OpenGL/OpenGLSpecification.hpp"

namespace IKan
{
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
    
    Renderer::Submit([this] {
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
      glDeleteTextures(1, &m_rendererID);
    });
  }
} // namespace IKan
