//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

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
  };
} // namespace IKan
