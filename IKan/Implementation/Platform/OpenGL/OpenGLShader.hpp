//
//  OpenGLShader.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include <glad/glad.h>

#include "Renderer/Graphics/Shader.hpp"

namespace IKan
{
  /// This class implements the APIs for for compiling and storing Open GL Shader.
  class OpenGLShader : public Shader
  {
  public:
    /// This consturctor creates the shader. Rarse and compile the shader code
    /// - Parameter shaderFilePath: Shader Code file path
    OpenGLShader(const std::filesystem::path& shaderFilePath);
    /// This destructor deletes the Open GL shader
    ~OpenGLShader();
  
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLShader);
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function reads the shader code in string and store all the shader code in a map to be used by compiler.
    /// - Parameter sourceString: shader code as string
    void PreprocessShader(const std::string& sourceString);
    /// This functions compiles all the shader codes and store their ID in Program ID (renderer_id).
    void Compile();

    // Member Variables ----------------------------------------------------------------------------------------------
    RendererID m_rendererID {0};
    std::filesystem::path m_filePath {};
    std::string m_name {};
    
    std::unordered_map<GLenum /* GL Shader type */, std::string /* Shader code */> m_shaderSourceCodeMap;
  };
} // namespace IKan
