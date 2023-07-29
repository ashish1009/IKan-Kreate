//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
  enum class TextureFormat
  {
    None = 0,
    RGBA8, RGBA, RED,
    R32I, RED_INTEGER,
    DEPTH_COMPONENT
  };
  
  enum class TextureType
  {
    None = 0,
    Texture2D,
  };
  
  enum class TextureFilter
  {
    None = 0,
    Linear,
    Nearest,
  };
  
  enum class TextureWrap
  {
    None = 0,
    Clamp,
    Repeat
  };
  
  enum class TextureAttachment
  {
    None = 0,
    Color, Depth
  };

  class Image
  {
  public:
    /// This is default virtual destructor for Texture
    virtual ~Image() = default;
    
    /// This static functions creates the Texture from image file
    /// - Parameters:
    ///   - filePath: path of texture file
    ///   - linear: min linear flag
    /// - Note: Use Renderer::GetTexture() to reuse already loaded texture (if following texture is already used
    ///         somewhere). This API will Load the texture again and new memory will be creted. So to avoid duplicate
    ///         memory for same texture use Renderer::GetTexture()
    [[nodiscard]] static Ref<Image> Create(const std::string& filePath, bool linear = true);
  };
} // namespace IKan
