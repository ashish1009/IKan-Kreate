//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include <glad/glad.h>
#include <stb_image.h>

#include "OpenGLTexture.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan
{
  // Utils ---------------------------------------------------------------------------------------------
  namespace TextureUtils
  {
    /// This function convert the IKan Format with GL Texture format
    /// - Parameter format: IKan Texture format
    GLint OpenGLFormatFromIKanFormat(TextureFormat format)
    {
      switch (format)
      {
        case TextureFormat::RGBA16F         : return GL_RGBA16F;
        case TextureFormat::RGBA8           : return GL_RGBA8;
        case TextureFormat::RGBA            : return GL_RGBA;
        case TextureFormat::RGB8            : return GL_RGB8;
        case TextureFormat::RGB             : return GL_RGB;
        case TextureFormat::R32I            : return GL_R32I;
        case TextureFormat::RED             : return GL_RED;
        case TextureFormat::RED_INTEGER     : return GL_RED_INTEGER;
        case TextureFormat::DEPTH_COMPONENT : return GL_DEPTH_COMPONENT;
        default:
          IK_ASSERT(false, "Invalid Format");
      }
    }
    /// This function convert the IKan Format with GL Texture format
    /// - Parameter format: IKan Texture format
    GLint OpenGLTypeFromIKanType(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D : return GL_TEXTURE_2D;
        case TextureType::TextureCubemap : return GL_TEXTURE_CUBE_MAP;
        default:
          IK_ASSERT(false, "Invalid Format");
      }
    }
    
    /// This function returns the data type of format from GL Texture format
    /// - Parameter formatType: GL Texture format
    GLint GetTextureDataType(GLint formatType)
    {
      switch (formatType)
      {
        case GL_RGBA8:
        case GL_RGB8:
        case GL_RGBA:
        case GL_RED:
        case GL_R32I:
          return GL_UNSIGNED_BYTE;
          
        case GL_DEPTH_COMPONENT:
        case GL_RGBA16F:
          return GL_FLOAT;
          
        default:
          IK_ASSERT(false, "Add other formats");
      }
    }
    /// This function returns the Open GL Filter from IKan Filter
    /// - Parameter filter: IKan Filter
    GLint OpenGLFilterFromIKanFilter(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:               return GL_LINEAR;
        case TextureFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
        case TextureFilter::Nearest:              return GL_NEAREST;
        default:
          IK_ASSERT(false, "Invalid Texture Filter");
      }
    }
    
    /// This function returns the Open GL Wrap from IKan Filter
    /// - Parameter filter: IKan Filter
    GLint OpenGLWrapFromIKanWrap(TextureWrap wrap)
    {
      switch (wrap)
      {
        case TextureWrap::ClampEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampBorder:  return GL_CLAMP_TO_BORDER;
        case TextureWrap::Repeat:       return GL_REPEAT;
        case TextureWrap::RepeatMirror: return GL_MIRRORED_REPEAT;
        default:
          IK_ASSERT(false, "Invalid Texture Wrap");
      }
    }
    
    const char* IKanFormatName(TextureFormat format)
    {
      switch (format)
      {
        case TextureFormat::RGBA16F :         return "GL_RGBA16F";
        case TextureFormat::RGBA8 :           return "GL_RGBA8";
        case TextureFormat::RGBA :            return "GL_RGBA";
        case TextureFormat::RGB8 :            return "GL_RGB8";
        case TextureFormat::RGB :             return "GL_RGB";
        case TextureFormat::RED :             return "GL_RED";
        case TextureFormat::R32I :            return "GL_R32I";
        case TextureFormat::RED_INTEGER :     return "GL_RED_INTEGER";
        case TextureFormat::DEPTH_COMPONENT : return "GL_DEPTH_COMPONENT";
        default:
          IK_ASSERT(false, "Invalid Format");
      }
    }
    const char* IKanWrapName(TextureWrap wrap)
    {
      switch (wrap) {
        case TextureWrap::ClampEdge:    return "GL_CLAMP_TO_EDGE";
        case TextureWrap::ClampBorder:  return "GL_CLAMP_TO_BORDER";
        case TextureWrap::Repeat:       return "GL_REPEAT";
        case TextureWrap::RepeatMirror: return "GL_MIRRORED_REPEAT";
        default:
          IK_ASSERT(false, "Invalid Texture Wrap");
      }
    }
    const char* IKanTypeName(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D: return "GL_TEXTURE_2D";
        case TextureType::TextureCubemap: return "GL_TEXTURE_CUBE_MAP";
        default:
          IK_ASSERT(false, "Invalid Texture Type");
      }
    }
    const char* IKanFilterName(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:               return "GL_LINEAR";
        case TextureFilter::Nearest:              return "GL_NEAREST";
        case TextureFilter::LinearMipmapLinear:   return "GL_LINEAR_MIPMAP_LINEAR";
        default:
          IK_ASSERT(false, "Invalid Texture Filter");
      }
    }
    
    /// This function converts the GL  Format to Ikan Format
    /// - Parameter format:GL Format
    TextureFormat OpenGLFormatToIkanFormat(GLint format)
    {
      if (format == GL_RGBA8)     return TextureFormat::RGBA8;
      else if (format == GL_RGBA) return TextureFormat::RGBA;
      else if (format == GL_RGB8) return TextureFormat::RGB8;
      else if (format == GL_RGB)  return TextureFormat::RGB;
      else if (format == GL_RED)  return TextureFormat::RED;
      
      IK_ASSERT(false, "Invalid Format");
    }
    
    /// This function returns the Attachment ID for Color or Depth
    /// - Parameters:
    ///   - attachment: Attachment type
    ///   - id: ID
    GLint IkanAttachmentToOpenGL(TextureAttachment attachment, uint32_t id)
    {
      switch (attachment)
      {
        case TextureAttachment::Color : return GL_COLOR_ATTACHMENT0 + id;
        case TextureAttachment::Depth : return GL_DEPTH_ATTACHMENT;
        default:
          IK_ASSERT(false, "Invalid Attachment");
      }
    }
  } // namespace TextureUtils
  
  // Open GL Texture ------------------------------------------------------------------------------------
  OpenGLTexture::OpenGLTexture(const Texture2DSpecification& spec)
  : m_specification(spec)
  {
    // Get GL Texture Type
    GLint textureType = TextureUtils::OpenGLTypeFromIKanType(m_specification.type);
    
    // Create Texture Buffer
    glGenTextures(1, &m_rendererID);
    glBindTexture(textureType, m_rendererID);
    
    // Check the channel of Image data if data is non zero
    if (m_specification.data)
    {
      m_channel = m_specification.dataFormat == TextureFormat::RGBA ? 4 : 3;
      IK_ASSERT((m_specification.size == (uint32_t)m_specification.width * (uint32_t)m_specification.height * m_channel), "Data must be entire texture");
    }
    
    // Create texture in the renderer Buffer
    GLint internalFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.internalFormat);
    GLint dataFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.dataFormat);
    GLint dataType = TextureUtils::GetTextureDataType(internalFormat);
    
    if (m_specification.type == TextureType::TextureCubemap)
    {
      for (uint32_t i = 0; i < 6; ++i)
      {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_specification.width, m_specification.height, 0, dataFormat, dataType, m_specification.data);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
      }
    }
    else
    {
      glTexImage2D(textureType, 0, /* Level */ internalFormat, m_specification.width, m_specification.height, 0 /* Border */, dataFormat, dataType, m_specification.data);
    }
    // Set the Texture parametes
    GLint filter = TextureUtils::OpenGLFilterFromIKanFilter(m_specification.filter);
    GLint wrap = TextureUtils::OpenGLWrapFromIKanWrap(m_specification.wrap);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filter);
    
    glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrap);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrap);
    
    // Increment the size in stats
    RendererStatistics::Get().textureBufferSize += m_specification.size;
    
    if (m_specification.data)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Texture 2D ");
      IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID     {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::Texture, "  Size            {0} B", m_specification.size);
      IK_LOG_DEBUG(LogModule::Texture, "  Channels        {0}", m_channel);
      IK_LOG_DEBUG(LogModule::Texture, "  Internal Format {0}", TextureUtils::IKanFormatName(m_specification.internalFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Data Format     {0}", TextureUtils::IKanFormatName(m_specification.dataFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Type            {0}", TextureUtils::IKanTypeName(m_specification.type));
      IK_LOG_DEBUG(LogModule::Texture, "  Wrap            {0}", TextureUtils::IKanWrapName(m_specification.wrap));
      IK_LOG_DEBUG(LogModule::Texture, "  Filter          {0}", TextureUtils::IKanFilterName(m_specification.filter));
      IK_LOG_DEBUG(LogModule::Texture, "  Total Size in GPU  {0} B ({1} KB, {2} MB)",
                   RendererStatistics::Get().textureBufferSize,
                   RendererStatistics::Get().textureBufferSize / 1000,
                   RendererStatistics::Get().textureBufferSize / 1000000);
    }
  }
  
  OpenGLTexture::~OpenGLTexture()
  {
    if (m_specification.data)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Texture 2D ");
      IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID     {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::Texture, "  Size            {0} B", m_specification.size);
      IK_LOG_DEBUG(LogModule::Texture, "  Channels        {0}", m_channel);
      IK_LOG_DEBUG(LogModule::Texture, "  Internal Format {0}", TextureUtils::IKanFormatName(m_specification.internalFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Data Format     {0}", TextureUtils::IKanFormatName(m_specification.dataFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Type            {0}", TextureUtils::IKanTypeName(m_specification.type));
      IK_LOG_DEBUG(LogModule::Texture, "  Wrap            {0}", TextureUtils::IKanWrapName(m_specification.wrap));
      IK_LOG_DEBUG(LogModule::Texture, "  Filter          {0}", TextureUtils::IKanFilterName(m_specification.filter));
    }
  }
  
  void OpenGLTexture::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(TextureUtils::OpenGLTypeFromIKanType(m_specification.type), m_rendererID);
  }
  
  void OpenGLTexture::Unbind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  void OpenGLTexture::AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID, uint32_t depthID, uint32_t level) const
  {
    if (m_specification.type == TextureType::TextureCubemap)
    {
      if (m_specification.internalFormat == TextureFormat::DEPTH_COMPONENT)
      {
        glFramebufferTexture(GL_FRAMEBUFFER,
                             TextureUtils::IkanAttachmentToOpenGL(attachmentType, colorID),
                             m_rendererID,
                             0 /* level */ );
      }
      else
      {
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               TextureUtils::IkanAttachmentToOpenGL(attachmentType, colorID),
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + depthID,
                               m_rendererID,
                               level);
      }
    }
    else
    {
      glFramebufferTexture2D(GL_FRAMEBUFFER, /* target */
                             TextureUtils::IkanAttachmentToOpenGL(attachmentType, colorID),
                             GL_TEXTURE_2D,
                             m_rendererID,
                             0 /* level */ );
    }
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
    m_name = m_specification.filePath.filename();
    
    // Invert Image
    if (spec.invertVertically)
    {
      stbi_set_flip_vertically_on_load(1);
    }
    
    // Load the file with stb image API
    void* data = stbi_load(m_specification.filePath.c_str(), &m_width, &m_height, &m_channel, 0 /* desired_channels */);
    // If file loaded successfullY
    if (data)
    {
      TextureFormat internalFormat = TextureFormat::RGBA8;
      TextureFormat dataFormat = TextureFormat::RGBA;
      
      switch (m_channel)
      {
        case 4 :
          internalFormat = TextureUtils::OpenGLFormatToIkanFormat(GL_RGBA8);
          dataFormat     = TextureUtils::OpenGLFormatToIkanFormat(GL_RGBA);
          break;
        case 3 :
          internalFormat = TextureUtils::OpenGLFormatToIkanFormat(GL_RGB8);
          dataFormat     = TextureUtils::OpenGLFormatToIkanFormat(GL_RGB);
          break;
        case 2 :
        case 1 :
          internalFormat = TextureUtils::OpenGLFormatToIkanFormat(GL_RED);
          dataFormat     = TextureUtils::OpenGLFormatToIkanFormat(GL_RED);
          break;
          
        default:
          IK_ASSERT(false, "Invalid Format ");
      }
      
      glGenTextures(1, &m_rendererID);
      glBindTexture(GL_TEXTURE_2D, m_rendererID);
      
      // Set the Texture parametes
      GLint filter = TextureUtils::OpenGLFilterFromIKanFilter(m_specification.filter);
      GLint wrap = TextureUtils::OpenGLWrapFromIKanWrap(m_specification.wrap);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
      
      // Create texture in the renderer Buffer
      GLint glInternalFormat = TextureUtils::OpenGLFormatFromIKanFormat(internalFormat);
      GLint glDataFormat = TextureUtils::OpenGLFormatFromIKanFormat(dataFormat);
      
      glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ glInternalFormat, m_width, m_height, 0, /* Border */ glDataFormat,
                   TextureUtils::GetTextureDataType(glInternalFormat), data);
      
      // Store the size of texture in Data
      m_size = (uint32_t)m_width * (uint32_t)m_height * (uint32_t)m_channel;
      
      // Increment the size in stats
      RendererStatistics::Get().textureBufferSize += m_size;
      
      // Delete the data as we have already loaded in graphics
      delete (stbi_uc*)data;
      
      IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Image Texture from File ... ");
      IK_LOG_DEBUG(LogModule::Texture, "  File Path          {0}", Utils::FileSystem::IKanAbsolute(m_specification.filePath));
      IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID        {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::Texture, "  Width              {0}", m_width);
      IK_LOG_DEBUG(LogModule::Texture, "  Height             {0}", m_height);
      IK_LOG_DEBUG(LogModule::Texture, "  Size               {0} B", m_size);
      IK_LOG_DEBUG(LogModule::Texture, "  Number of Channel  {0}", m_channel);
      IK_LOG_DEBUG(LogModule::Texture, "  Internal Format    {0}", TextureUtils::IKanFormatName(internalFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Data Format        {0}", TextureUtils::IKanFormatName(dataFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Wrap               {0}", TextureUtils::IKanWrapName(m_specification.wrap));
      IK_LOG_DEBUG(LogModule::Texture, "  Filter             {0}", TextureUtils::IKanFilterName(m_specification.filter));
      IK_LOG_DEBUG(LogModule::Texture, "  Total Size in GPU  {0} B ({1} KB, {2} MB",
                   RendererStatistics::Get().textureBufferSize,
                   RendererStatistics::Get().textureBufferSize / 1000,
                   RendererStatistics::Get().textureBufferSize / 1000000);
    } // if data
    else
    {
      IK_ASSERT(false);
      IK_LOG_ERROR(LogModule::Texture, "File Do not Exist ");
    }
  }
  
  OpenGLImage::~OpenGLImage()
  {
    IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Image Texture ");
    IK_LOG_DEBUG(LogModule::Texture, "  File Path          {0}", Utils::FileSystem::IKanAbsolute(m_specification.filePath));
    IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID        {0}", m_rendererID);
    
    RendererStatistics::Get().textureBufferSize -= m_size;
  }
  
  void OpenGLImage::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
  }
  
  void OpenGLImage::Unbind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  uint32_t OpenGLImage::GetWidth() const
  {
    return m_width;
  }
  
  uint32_t OpenGLImage::GetHeight() const
  {
    return m_height;
  }
  
  RendererID OpenGLImage::GetRendererID() const
  {
    return m_rendererID;
  }
  
  const std::filesystem::path& OpenGLImage::GetfilePath() const
  {
    return m_specification.filePath;
  }
  
  const std::string& OpenGLImage::GetName() const
  {
    return m_name;
  }
} // namespace IKan
