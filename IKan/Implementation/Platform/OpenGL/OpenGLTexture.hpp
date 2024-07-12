//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// This class implements the APIs to create texture for Open GL.
  class OpenGLTexture : public Texture
  {
  public:
    /// This constructor creates the Open GL texture from specificaion data.
    /// - Parameter spec: Open GL Texture specification data.
    OpenGLTexture(const Texture2DSpecification& spec);
    /// This destructor destroyes and deletes the Open GL texture.
    ~OpenGLTexture();
  };
} // namespace IKan
