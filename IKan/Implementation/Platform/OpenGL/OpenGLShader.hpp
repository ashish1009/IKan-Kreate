//
//  OpenGLShader.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include <glad/glad.h>
#include "Renderer/Graphics/Shader.hpp"

namespace IKan
{
  // Max Supported Shaders
  static constexpr uint32_t MaxShaderSupported = 3;
  
  /// Interface class for storing Shader
  class OpenGLShader : public Shader
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter shaderFilePath: Shader Code file path
    OpenGLShader(const std::filesystem::path& shaderFilePath);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLShader();
  };
} // namespace IKan
