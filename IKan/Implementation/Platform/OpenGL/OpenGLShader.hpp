//
//  OpenGLShader.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include <glad/glad.h>
#include "Renderer/Graphics/Shader.hpp"
#include "Platform/OpenGL/OpenGLShaderUniform.hpp"

namespace IKan
{
  /// This is the Implementation class for Shader compiler for Open GL
  class OpenGLShader : public Shader
  {
  public:
    /// This is the constructo to create Open GL Shader instance
    /// - Parameter shaderFilePath: Shader Code full file path
    OpenGLShader(const std::string& shaderFilePath);
    /// This is the default Open GL Shader Destructor
    ~OpenGLShader();
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    RendererID m_rendererID;
    std::string m_filePath;
    std::string m_name;
  };
} // namespace IKan
