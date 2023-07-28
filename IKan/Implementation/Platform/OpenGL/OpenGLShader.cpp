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

  namespace ShaderUtils
  {
    /// This function returns the Sgader type in Open GL Format enum
    /// - Parameter type: type of shader in string (e.g. "vertex" or "fragment" ... )
    static GLenum ShaderTypeFromString(const std::string& type)
    {
      if ("vertex" == type)   return GL_VERTEX_SHADER;
      if ("fragment" == type) return GL_FRAGMENT_SHADER;
      if ("geometry" == type) return GL_GEOMETRY_SHADER;
      IK_ASSERT(false, "Unknown shader type!");
    }
  } // namespace ShaderUtils
  
  OpenGLShader::OpenGLShader(const std::string& shaderFilePath)
  : m_rendererID(glCreateProgram()), m_filePath(shaderFilePath), m_name(Utils::String::GetFileNameFromPath(shaderFilePath))
  {
    IK_PROFILE();
    SHADER_LOG("Creating Open GL Shader ");
    SHADER_LOG("------------------------");
    SHADER_LOG("  Renderer ID  {0} ", m_rendererID);
    SHADER_LOG("  Name         {0} ", m_name);
    SHADER_LOG("  File Path    {0} ", m_filePath);
    
    // Extract the file and store the glsl code as text string
    std::string shaderString = Utils::String::ReadFromFile(m_filePath);
    IK_ASSERT(shaderString != "", "File Not exist");
    
    // Process the shader codes
    PreprocessShader(shaderString);
  }
  OpenGLShader::~OpenGLShader()
  {
    IK_PROFILE();
    SHADER_LOG("Destroying Open GL Shader ");
    SHADER_LOG("--------------------------");
    SHADER_LOG("  Renderer ID  {0} ", m_rendererID);
    SHADER_LOG("  Name         {0} ", m_name);
    SHADER_LOG("  File Path    {0} ", m_filePath);
  }
  
  void OpenGLShader::PreprocessShader(const std::string &sourceString)
  {
    SHADER_LOG("  Processing {0}", m_name.c_str());
    // All type (Fragment, Vertex, Geomatry etc...) of shader code should started by this token (e.g. '#type vertex'
    // for vertex shader). Find the token("#type") position
    const char* token = "#type";
    size_t tokenPos = sourceString.find(token);
    
    // Keep searching for token until we each end of shader code
    while (tokenPos != std::string::npos)
    {
      // Fine end of line of "#type <type>"
      size_t tokenEol = sourceString.find_first_of("\r\n", tokenPos);
      IK_ASSERT((tokenEol != std::string::npos), "Syntax error");
      
      // points after the token "#type " e.g. "#type vertex" it will point at 'v'
      size_t typeBegin = tokenPos + strlen(token) + 1; // Just after the "#type "
      
      // store the type of shader (either vertex or Fragment or Geomatry etc ) from 'v' to 'x' in e.g "#type vertex"
      std::string type = sourceString.substr(typeBegin, tokenEol - typeBegin);
      IK_ASSERT(ShaderUtils::ShaderTypeFromString(type), "Invalid shader type specified");
      
      // First line of Shader code. This is just next line after (#type <name>)
      size_t firstLine = sourceString.find_first_of("\r\n", tokenEol);
      
      // Finds the position of next "#type", which is the start of next shader code and end of current shader code.
      // if not found the word "#type" that means this is the last shade code so find the eof
      tokenPos = sourceString.find(token, firstLine);
      
      // Extract the code string from 'shader_code_first_line' (which is the first line of shader code) to the end of
      // current shader code using variable 'token_pos' (either next token or eol) and store the code in as string in map
      m_shaderSourceCodeMap[ShaderUtils::ShaderTypeFromString(type)] =
      sourceString.substr(firstLine, tokenPos - (firstLine == std::string::npos ? sourceString.size() - 1 : firstLine));
    } // While all token
    
    // if Not able to read the file then no shader is loaded in the map
    IK_ASSERT(m_shaderSourceCodeMap.size(), "Shader source empty. No Shader exist");
  }

} // namespace IKan
