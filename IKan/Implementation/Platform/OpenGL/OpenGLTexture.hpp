//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <glad/glad.h>
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// This is the Open GL Implementation class for Creating texture
  class OpenGLImage : public Image
  {
  public:
    /// This constructor creates the texture using image file path
    /// - Parameters:
    ///   - filePath: Texture image file path (absolute)
    ///   - linear: min linear filter
    OpenGLImage(const std::string& filePath, bool linear);
    /// Default destructor that delete the texture
    virtual ~OpenGLImage();
    
  private:
    RendererID m_rendererID = 0;
    int32_t m_width = 1600;
    int32_t m_height = 900;
    int32_t m_channel = 0;
    uint32_t m_size = 0;
    std::string m_filePath = "", m_name = "";
  };
  
  namespace TextureUtils
  {
    /// This function returns the Format name from Enum
    /// - Parameter format: enum taken as uint
    std::string IKanFormatName(TextureFormat format);
  } // namespace TextureUtils

} // namespace IKan
