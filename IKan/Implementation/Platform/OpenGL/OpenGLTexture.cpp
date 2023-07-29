//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "OpenGLTexture.hpp"

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
  {
    
  }
  
  OpenGLImage::~OpenGLImage()
  {
    
  }
}
