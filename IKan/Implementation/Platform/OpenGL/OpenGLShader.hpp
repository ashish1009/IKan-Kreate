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
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function reads the shader code in string and store all the shader present in the file in a map to be used
    /// later by compiler.
    /// - Parameter sourceString: shader code as string
    void PreprocessShader(const std::string& sourceString);
    /// This functions compiles all the shader codes and store their ID in Program ID (renderer_id).
    void Compile();

    // Member Variables ---------------------------------------------------------------------------------------------
    RendererID m_rendererID;
    std::string m_filePath;
    std::string m_name;
    std::unordered_map<GLenum /* GL Shader type */, std::string /* Shader code  */> m_shaderSourceCodeMap;
  };
} // namespace IKan
