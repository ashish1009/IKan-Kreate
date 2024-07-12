//
//  OpenGLSpecification.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include <glad/glad.h>

#include "Renderer/Graphics/Specifications.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  // Texture utils ---------------------------------------------------------------
  namespace TextureUtils
  {
    /// This function converts the IKan texture type with GL texture type
    /// - Parameter type: IKan texture type
    GLenum OpenGLTypeFromIKanType(TextureType type);
    
    /// This function converts the IKan texture format with GL texture format
    /// - Parameter format: IKan texture format
    GLenum OpenGLFormatFromIKanFormat(TextureFormat format);

    /// This function returns the data type of GL texture format type
    /// - Parameter formatType: GL texture type
    GLenum GetTextureDataType(GLenum formatType);

    /// This function converts the IKan texture filter with GL texture filter
    /// - Parameter filter: IKan filter
    GLint OpenGLFilterFromIKanFilter(TextureFilter filter);

    /// This function converts the IKan texture wrap with GL texture wrap
    /// - Parameter wrap: IKan wrap
    GLint OpenGLWrapFromIKanWrap(TextureWrap wrap);

    /// This function returns the texture format name as const char *
    /// - Parameter format: format type
    std::string_view IKanFormatName(TextureFormat format);
    
    /// This function returns the texture wrap name as const char *
    /// - Parameter wrap: texture wrap
    std::string_view IKanWrapName(TextureWrap wrap);
    
    /// This function returns the texture type name as const char *
    /// - Parameter type: texture type
    std::string_view IKanTypeName(TextureType type);
    
    /// This function returns the texture filter name as const char *
    /// - Parameter filter: filter
    std::string_view IKanFilterName(TextureFilter filter);
  } // namespace TextureUtils
} // namespace IKan
