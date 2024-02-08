//
//  OpenGLTexture.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include <glad/glad.h>

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
} // namespace IKan
