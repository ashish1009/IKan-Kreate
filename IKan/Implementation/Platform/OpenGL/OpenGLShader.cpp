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
  : m_rendererID(glCreateProgram()), m_filePath(shaderFilePath), m_name(shaderFilePath.filename())
  {
    IK_PROFILE();
    IK_ASSERT(shaderFilePath != "", "Empty File Path");
    SHADER_LOG("Creating '{0}' for Open GL Shader (ID : {1})", m_name, m_rendererID);
    
    // Extract the file and store the glsl code as text string
    std::string shaderString = Utils::String::ReadFromFile(m_filePath);
    IK_ASSERT(shaderString != "", "Empty File Content");
  }
  
  OpenGLShader::~OpenGLShader()
  {
    IK_PROFILE();
    SHADER_LOG("Destroying '{0}' for Open GL Shader (ID : {1})", m_name, m_rendererID);
  }
} // namespace IKan
