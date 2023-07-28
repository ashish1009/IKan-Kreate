//
//  OpenGLShader.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "OpenGLShader.hpp"

namespace IKan
{
#define SHADER_LOG(...) IK_LOG_DEBUG(LogModule::Shader, __VA_ARGS__);

  OpenGLShader::OpenGLShader(const std::string& shaderFilePath)
  : m_rendererID(glCreateProgram()), m_filePath(shaderFilePath), m_name(Utils::String::GetFileNameFromPath(shaderFilePath))
  {
    IK_PROFILE();
    SHADER_LOG("Creating Open GL Shader ");
    SHADER_LOG("-----------------------------");
    SHADER_LOG("  Renderer ID  {0} ", m_rendererID);
    SHADER_LOG("  Name         {0} ", m_name);
    SHADER_LOG("  File Path    {0} ", m_filePath);
  }
  OpenGLShader::~OpenGLShader()
  {
    IK_PROFILE();
    SHADER_LOG("Destroying Open GL Shader ");
    SHADER_LOG("-----------------------------");
    SHADER_LOG("  Renderer ID  {0} ", m_rendererID);
    SHADER_LOG("  Name         {0} ", m_name);
    SHADER_LOG("  File Path    {0} ", m_filePath);
  }
} // namespace IKan
