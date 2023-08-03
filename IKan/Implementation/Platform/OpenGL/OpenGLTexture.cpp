//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include <stb_image.h>
#include "OpenGLTexture.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan
{
  namespace TextureUtils
  {
    [[maybe_unused]] std::string IKanFormatName(TextureFormat format)
    {
      switch (format)
      {
        case TextureFormat::RGBA8 :           return "GL_RGBA8";
        case TextureFormat::RGBA :            return "GL_RGBA";
        case TextureFormat::RGB8 :            return "GL_RGB8";
        case TextureFormat::RGB :             return "GL_RGB";
        case TextureFormat::R32I :            return "GL_R32I";
        case TextureFormat::RED_INTEGER :     return "GL_RED_INTEGER";
        case TextureFormat::DEPTH_COMPONENT : return "GL_DEPTH_COMPONENT";
        default:
          IK_ASSERT(false, "Invalid Format");
      }
    }
    [[maybe_unused]] std::string IKanWrapName(TextureWrap wrap)
    {
      switch (wrap) {
        case TextureWrap::Clamp:  return "GL_CLAMP_TO_EDGE";
        case TextureWrap::Repeat: return "GL_REPEAT";
        default:
          IK_ASSERT(false, "Invalid Texture Wrap");
      }
    }
    [[maybe_unused]] std::string IKanTypeName(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D: return "GL_TEXTURE_2D";
        case TextureType::None:
        default:
          IK_ASSERT(false, "Invalid Texture Type");
      }
    }
    std::string IKanFilterName(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:   return "GL_LINEAR";
        case TextureFilter::Nearest:  return "GL_NEAREST";
        default:
          IK_ASSERT(false, "Invalid Texture Filter");
      }
    }
    
    /// This function convert the IKan Format with GL Texture format
    /// - Parameter format: IKan Texture format
    [[maybe_unused]] GLint OpenGLFormatFromIKanFormat(TextureFormat format)
    {
      switch (format)
      {
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
    
    /// This function returns the data type of format from GL Texture format
    /// - Parameter formatType: GL Texture format
    [[maybe_unused]] GLint GetTextureType(GLint formatType)
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
          return GL_FLOAT;
          
        default:
          IK_ASSERT(false, "Add other formats");
      }
    }
    
    /// This function returns the Open GL type from IKan Type
    /// - Parameter type: IKan type
    [[maybe_unused]] GLint OpenGLTypeFromIKanType(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D: return GL_TEXTURE_2D;
        case TextureType::None:
        default:
          IK_ASSERT(false, "Invalid Texture Type");
      }
    }
    
    /// This function returns the Open GL Filter from IKan Filter
    /// - Parameter filter: IKan Filter
    [[maybe_unused]] GLint OpenGLFilterFromIKanFilter(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:   return GL_LINEAR;
        case TextureFilter::Nearest:  return GL_NEAREST;
        default:
          IK_ASSERT(false, "Invalid Texture Filter");
      }
    }
    
    /// This function returns the Open GL Wrap from IKan Filter
    /// - Parameter filter: IKan Filter
    [[maybe_unused]] GLint OpenGLWrapFromIKanFilter(TextureWrap wrap)
    {
      switch (wrap) {
        case TextureWrap::Clamp:  return GL_CLAMP_TO_EDGE;
        case TextureWrap::Repeat: return GL_REPEAT;
        default:
          IK_ASSERT(false, "Invalid Texture Wrap");
      }
    }
    /// This function converts the GL  Format to Ikan Format
    /// - Parameter format:GL Format
    [[maybe_unused]] TextureFormat OpenGLFormatToIkanFormat(GLint format)
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
    [[maybe_unused]] GLint IkanAttachmentToOpenGL(TextureAttachment attachment, uint32_t id)
    {
      switch (attachment)
      {
        case TextureAttachment::Color : return GL_COLOR_ATTACHMENT0 + id;
        case TextureAttachment::Depth : return GL_DEPTH_ATTACHMENT;
        default:
          IK_ASSERT(false, "Invalid Attachment");
      }
    }
  } // namespace textureUtils
  
  OpenGLImage::OpenGLImage(const std::string& filePath, bool linear)
  : m_filePath(filePath), m_name(Utils::String::GetFileNameFromPath(filePath))
  {
#ifdef InvertVertically
    // Invert the texture. as by default open gl load inverted vertically
    stbi_set_flip_vertically_on_load(1);
#endif
    
    // Load the file with stb image API
    void* data = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_channel, 0 /* desired_channels */);
    
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
      
      TextureType type = TextureType::Texture2D;
      TextureWrap wrap = TextureWrap::Repeat;
      TextureFilter filter = (linear) ? TextureFilter::Linear : TextureFilter::Nearest;
      
      // Setup min and Mag filter
      glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(type), GL_TEXTURE_MIN_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(filter));
      glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(type), GL_TEXTURE_MAG_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(filter));
      
      // Texuter Flags
      glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(type), GL_TEXTURE_WRAP_R, TextureUtils::OpenGLWrapFromIKanFilter(wrap));
      glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(type), GL_TEXTURE_WRAP_S, TextureUtils::OpenGLWrapFromIKanFilter(wrap));
      glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(type), GL_TEXTURE_WRAP_T, TextureUtils::OpenGLWrapFromIKanFilter(wrap));
      
      // Create texture in the renderer Buffer
      GLint glInternalFormat = TextureUtils::OpenGLFormatFromIKanFormat(internalFormat);
      GLint glDataFormat = TextureUtils::OpenGLFormatFromIKanFormat(dataFormat);
      
      glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ glInternalFormat, m_width, m_height, 0, /* Border */ glDataFormat,
                   TextureUtils::GetTextureType(glInternalFormat), data);
      
      // Store the size of texture in Data
      m_size = (uint32_t)m_width * (uint32_t)m_height * (uint32_t)m_channel;
      
      // Increment the size in stats
      RendererStatistics::Get().textureBufferSize += m_size;
      
      // Delete the data as we have already loaded in graphics
      delete (stbi_uc*)data;
      
      IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Texture from File ... ");
      IK_LOG_DEBUG(LogModule::Texture, "-------------------------------------- ");
      IK_LOG_DEBUG(LogModule::Texture, "  File Path          {0}", Utils::FileSystem::IKanAbsolute(m_filePath));
      IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID        {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::Texture, "  Width              {0}", m_width);
      IK_LOG_DEBUG(LogModule::Texture, "  Height             {0}", m_height);
      IK_LOG_DEBUG(LogModule::Texture, "  Size               {0} B", m_size);
      IK_LOG_DEBUG(LogModule::Texture, "  Number of Channel  {0}", m_channel);
      IK_LOG_DEBUG(LogModule::Texture, "  Internal Format    {0}", TextureUtils::IKanFormatName(internalFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Data Format        {0}", TextureUtils::IKanFormatName(dataFormat));
      IK_LOG_DEBUG(LogModule::Texture, "  Type               {0}", TextureUtils::IKanTypeName(type));
      IK_LOG_DEBUG(LogModule::Texture, "  Wrap               {0}", TextureUtils::IKanWrapName(wrap));
      IK_LOG_DEBUG(LogModule::Texture, "  Filter             {0}", TextureUtils::IKanFilterName(filter));
    }
    else
    {
      IK_ASSERT(false, "Failed to load stbi Image. Check the path from Logs");
    }
  }
  
  OpenGLImage::~OpenGLImage()
  {
    IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Image Texture ");
    IK_LOG_DEBUG(LogModule::Texture, "-------------------------------- ");
    IK_LOG_DEBUG(LogModule::Texture, "  File Path          {0}", Utils::FileSystem::IKanAbsolute(m_filePath));
    IK_LOG_DEBUG(LogModule::Texture, "  Renderer ID        {0}", m_rendererID);
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
  
  const std::string& OpenGLImage::GetfilePath() const
  {
    return m_filePath;
  }
  
  const std::string& OpenGLImage::GetName() const
  {
    return m_name;
  }
  
  OpenGLTexture::OpenGLTexture(const TextureSpecification& spec)
  : m_specification(spec)
  {
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
    // Check the channel of Image data if data is non zero
    if (m_specification.data)
    {
      m_channel = m_specification.dataFormat == TextureFormat::RGBA ? 4 : 3;
      IK_ASSERT((m_specification.size == (uint32_t)m_specification.width * (uint32_t)m_specification.height * m_channel),
                "Data must be entire texture");
    }
    
    // Create texture in the renderer Buffer
    GLint internalFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.internalFormat);
    GLint dataFormat = TextureUtils::OpenGLFormatFromIKanFormat(m_specification.dataFormat);
    
    // Create the GL texture from data
    glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ (GLint)internalFormat, m_specification.width, m_specification.height,
                 0 /* Border */, dataFormat, TextureUtils::GetTextureType(internalFormat), m_specification.data);
    
    // Set the Texture parametes
    glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(m_specification.type),
                    GL_TEXTURE_MIN_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.filter));
    glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(m_specification.type),
                    GL_TEXTURE_MAG_FILTER, TextureUtils::OpenGLFilterFromIKanFilter(m_specification.filter));
    
    glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(m_specification.type),
                    GL_TEXTURE_WRAP_R, TextureUtils::OpenGLWrapFromIKanFilter(m_specification.wrap));
    glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(m_specification.type),
                    GL_TEXTURE_WRAP_S, TextureUtils::OpenGLWrapFromIKanFilter(m_specification.wrap));
    glTexParameteri(TextureUtils::OpenGLTypeFromIKanType(m_specification.type),
                    GL_TEXTURE_WRAP_T, TextureUtils::OpenGLWrapFromIKanFilter(m_specification.wrap));
    
    // Increment the size in stats
    RendererStatistics::Get().textureBufferSize += m_specification.size;
    
    if (m_specification.data)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Creating Open GL Image Texture ");
      IK_LOG_DEBUG(LogModule::Texture, "------------------------------ ");
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
  
  OpenGLTexture::~OpenGLTexture()
  {
    if (m_specification.data)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Destroying Open GL Image Texture ");
      IK_LOG_DEBUG(LogModule::Texture, "------------------------------ ");
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
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
  }
  
  void OpenGLTexture::Unbind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  
  void OpenGLTexture::AttachToFramebuffer(TextureAttachment attachmentType, uint32_t id) const
  {
    glFramebufferTexture2D(GL_FRAMEBUFFER, /* target */
                           TextureUtils::IkanAttachmentToOpenGL(attachmentType, id),
                           GL_TEXTURE_2D,
                           m_rendererID,
                           0 /* level */ );
  }

  uint32_t OpenGLTexture::GetWidth() const
  {
    return m_specification.width;
  }
  
  uint32_t OpenGLTexture::GetHeight() const
  {
    return m_specification.height;
  }
  
  RendererID OpenGLTexture::GetRendererID() const
  {
    return m_rendererID;
  }
  
  OpenGLCharTexture::OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing,
                                       uint32_t advance, [[maybe_unused]] char charVal)
  : m_size(size), m_bearing(bearing), m_advance(advance)
  {
    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
    
    // Create texture in the renderer Buffer
    glTexImage2D(GL_TEXTURE_2D, 0, /* Level */ GL_RED, (GLsizei)face->glyph->bitmap.width, (GLsizei)face->glyph->bitmap.rows,
                 0, /* Border */ GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer );
    
    m_width = (uint32_t)face->glyph->bitmap.width;
    m_height = (uint32_t)face->glyph->bitmap.rows;
    
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
#if CHAR_TEXTURE_LOG
    IK_LOG_TRACE(LogModule::CharTexture, "Creating Open GL Char Texture to store Char {0} ( Renderer ID {1} )",
                  charVal, m_rendererID);
#endif
    
    // Increment the size in stats
    m_dataSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
    RendererStatistics::Get().textureBufferSize += m_dataSize;
  }
  
  OpenGLCharTexture::~OpenGLCharTexture()
  {
    glDeleteTextures(1, &m_rendererID);
    RendererStatistics::Get().textureBufferSize -= m_dataSize;
  }
  
  void OpenGLCharTexture::Bind(uint32_t slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
  }
  void OpenGLCharTexture::Unbind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
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
    return m_size;
  }
  glm::ivec2 OpenGLCharTexture::GetBearing() const
  {
    return m_bearing;
  }
  uint32_t OpenGLCharTexture::GetAdvance() const
  {
    return m_advance;
  }

} // namespace IKan
