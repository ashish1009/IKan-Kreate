//
//  OpenGLSpecification.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLSpecification.hpp"

#include "Renderer/Graphics/Texture.hpp"

// Utils ---------------------------------------------------------------------------------------------
namespace IKan
{
  namespace TextureUtils
  {
    /// This function converts the IKan Texture type with GL Texture type
    /// - Parameter format: IKan Texture type
    GLenum OpenGLTypeFromIKanType(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D      : return GL_TEXTURE_2D;
        case TextureType::TextureCubemap : return GL_TEXTURE_CUBE_MAP;
        default:
          IK_ASSERT(false, "Invalid Format!");
      }
      return GL_INVALID_INDEX;
    }
    
    /// This function converts the IKan Format with GL Texture format
    /// - Parameter format: IKan Texture format
    GLenum OpenGLFormatFromIKanFormat(TextureFormat format)
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
          IK_ASSERT(false, "Invalid Format!");
      }
      return GL_INVALID_INDEX;
    }

    /// This function returns the data type of format from GL Texture type
    /// - Parameter formatType: GL Texture type
    GLenum GetTextureDataType(GLenum formatType)
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
          IK_ASSERT(false, "Add other formats!");
      }
      return GL_INVALID_INDEX;
    }

    /// This function returns the Open GL Filter from IKan Filter
    /// - Parameter filter: IKan Filter
    GLint OpenGLFilterFromIKanFilter(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:                 return GL_LINEAR;
        case TextureFilter::Nearest:                return GL_NEAREST;
        case TextureFilter::LinearMipmapLinear:     return GL_LINEAR_MIPMAP_LINEAR;
        case TextureFilter::LinearMipmapNearest:    return GL_LINEAR_MIPMAP_NEAREST;
        case TextureFilter::NearestMipmapLinear:    return GL_NEAREST_MIPMAP_LINEAR;
        case TextureFilter::NearestMipmapNearest:   return GL_NEAREST_MIPMAP_NEAREST;
        default:
          IK_ASSERT(false, "Invalid Texture Filter!");
      }
      return (GLint)GL_INVALID_INDEX;
    }

    /// This function returns the Open GL Wrap from IKan wrap
    /// - Parameter filter: IKan wrap
    GLint OpenGLWrapFromIKanWrap(TextureWrap wrap)
    {
      switch (wrap)
      {
        case TextureWrap::ClampEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrap::ClampBorder:  return GL_CLAMP_TO_BORDER;
        case TextureWrap::Repeat:       return GL_REPEAT;
        case TextureWrap::RepeatMirror: return GL_MIRRORED_REPEAT;
        default:
          IK_ASSERT(false, "Invalid Texture Wrap!");
      }
      return (GLint)GL_INVALID_INDEX;
    }

    std::string_view IKanFormatName(TextureFormat format)
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
          IK_ASSERT(false, "Invalid Format!");
      }
      return "";
    }
    std::string_view IKanWrapName(TextureWrap wrap)
    {
      switch (wrap) {
        case TextureWrap::ClampEdge:    return "GL_CLAMP_TO_EDGE";
        case TextureWrap::ClampBorder:  return "GL_CLAMP_TO_BORDER";
        case TextureWrap::Repeat:       return "GL_REPEAT";
        case TextureWrap::RepeatMirror: return "GL_MIRRORED_REPEAT";
        default:
          IK_ASSERT(false, "Invalid Texture Wrap!");
      }
      return "";
    }
    std::string_view IKanTypeName(TextureType type)
    {
      switch (type)
      {
        case TextureType::Texture2D: return "GL_TEXTURE_2D";
        case TextureType::TextureCubemap: return "GL_TEXTURE_CUBE_MAP";
        default:
          IK_ASSERT(false, "Invalid Texture Type!");
      }
      return "";
    }
    std::string_view IKanFilterName(TextureFilter filter)
    {
      switch (filter)
      {
        case TextureFilter::Linear:               return "GL_LINEAR";
        case TextureFilter::Nearest:              return "GL_NEAREST";
        case TextureFilter::LinearMipmapLinear:   return "GL_LINEAR_MIPMAP_LINEAR";
        default:
          IK_ASSERT(false, "Invalid Texture Filter!");
      }
      return "";
    }

  } // namespace TextureUtils
} // namespace IKan
