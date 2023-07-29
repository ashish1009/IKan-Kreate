//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
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
