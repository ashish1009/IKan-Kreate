//
//  OpenGLShader.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "OpenGLShader.hpp"

namespace IKan 
{
#define SHADER_LOG(...) IK_LOG_DEBUG(LogModule::Shader, __VA_ARGS__);
  
  OpenGLShader::OpenGLShader(const std::filesystem::path& shaderFilePath)
  {
    IK_PROFILE();
    IK_ASSERT(shaderFilePath != "", "Empty File Path");
    SHADER_LOG("Creating '{0}' for Open GL Shader");

  }
  OpenGLShader::~OpenGLShader()
  {
    IK_PROFILE();
    SHADER_LOG("Destroying '{0}' for Open GL Shader");
  }
} // namespace IKan
