//
//  OpenGLShader.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

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
  };
} // namespace IKan
