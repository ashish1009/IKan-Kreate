//
//  OpenGLShader.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLShader.hpp"

namespace IKan
{
#define SHADER_LOG(...) IK_LOG_DEBUG(LogModule::Shader, __VA_ARGS__);
  
  // Shader Utils ----------------------------------------------------------------------------------------------------
  namespace ShaderUtils
  {
    /// This function returns the shader type in Open GL Format enum
    /// - Parameter type: type of shader in string (e.g. "vertex" or "fragment" ... )
    static GLenum ShaderTypeFromString(const std::string_view& type)
    {
      if ("vertex" == type)   return GL_VERTEX_SHADER;
      if ("fragment" == type) return GL_FRAGMENT_SHADER;
      if ("geometry" == type) return GL_GEOMETRY_SHADER;
      IK_ASSERT(false, "Unknown shader type!");
      return 0;
    }
  } // namespace ShaderUtils
  
  OpenGLShader::OpenGLShader(const std::filesystem::path& shaderFilePath)
  : m_rendererID(glCreateProgram()), m_filePath(shaderFilePath), m_name(shaderFilePath.filename())
  {
    Renderer::Submit([this](){
      IK_PROFILE();
      IK_ASSERT(m_filePath != "", "Empty File Path!");
      SHADER_LOG("Creating '{0}' for Open GL Shader (ID : {1}).", m_name, m_rendererID);
      
      // Extract the file and store the glsl code as text string
      std::string shaderString = Utils::String::ReadFromFile(m_filePath);
      IK_ASSERT(shaderString != "", "Empty File Content!");
      
      // Process the shader codes
      PreprocessShader(shaderString);
    });
  }
  
  OpenGLShader::~OpenGLShader()
  {
    SHADER_LOG("Destroying '{0}' for Open GL (ID : {1}).", m_name, m_rendererID);
  }
  
  void OpenGLShader::PreprocessShader(const std::string &sourceString)
  {
    IK_PROFILE();
    SHADER_LOG("  Preprocessing {0}", m_name.c_str());
    
    // All shader code (Vertex, Fragment or Geomatry) should start with "#type" keyword following by type of shader
    // (e.g. #type vertes)
    std::string_view token {"#type"};
    
    // Search for "#type" keyword in code
    size_t tokenPos = sourceString.find(token);
    
    // Find all token in code
    while (tokenPos != std::string::npos)
    {
      // Position after the token "#type " e.g. "#type vertex" it will point at 'v'
      size_t typePos = tokenPos + strlen(token.data()) + 1;
      
      // Position of end of line from #type
      size_t tokenEol = sourceString.find_first_of("\r\n", tokenPos);
      
      // Get the type of shader
      std::string type = sourceString.substr(typePos, tokenEol - typePos);
      IK_ASSERT(ShaderUtils::ShaderTypeFromString(type), "Invalid shader type specified");

      // Get the first line of code
      size_t firstLine = sourceString.find_first_of("\r\n", tokenEol);
      
      // Finds the position of next "#type", which is the start of next shader code and end of current shader code.
      // if not found the word "#type" that means this is the last shade code so find the eof
      tokenPos = sourceString.find(token, firstLine);
      
      // Extract the code string from 'shader_code_first_line' (which is the first line of shader code) to the end of
      // current shader code using variable 'token_pos' (either next token or eol) and store the code in as string in map
      m_shaderSourceCodeMap[ShaderUtils::ShaderTypeFromString(type)] =
      sourceString.substr(firstLine, tokenPos - (std::string::npos == firstLine ? sourceString.size() - 1 : firstLine));
    }
    
    // if Not able to read the file then no shader is loaded in the map
    IK_ASSERT(m_shaderSourceCodeMap.size(), "Shader source empty. No Shader exist");
  }
} // namespace IKan
