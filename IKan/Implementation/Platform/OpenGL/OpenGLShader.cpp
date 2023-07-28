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
    
    /// This function returns the Open GL Shader Name from Open GL Type
    /// - Parameter type: type of shader in GL enum
    [[maybe_unused]] static const char* ShaderNameFromType(GLenum type)
    {
      if (type == GL_VERTEX_SHADER) return "Vertex";
      if (type == GL_FRAGMENT_SHADER) return "Fragment";
      if (type == GL_GEOMETRY_SHADER) return "Geomatry";
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
    
    // Compile and Link the shader code
    Compile();

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

  void OpenGLShader::Compile()
  {
    SHADER_LOG("  Compiling  {0} ", m_name.c_str());
    
    // Check Mendatory Shaders : Vertex and Fragment are present
    IK_ASSERT(m_shaderSourceCodeMap.find(GL_VERTEX_SHADER) != m_shaderSourceCodeMap.end(), "Vertex Shader not loaded in file");
    IK_ASSERT(m_shaderSourceCodeMap.find(GL_FRAGMENT_SHADER) != m_shaderSourceCodeMap.end(), "Fragment Shader not loaded in file");
    
    // Stores the shader code ids
    std::vector<GLuint> shaderIDs;
    
    // Shader Compiler
    for (const auto& [shaderType, shaderSrc] : m_shaderSourceCodeMap)
    {
      // Generate the Shader ID
      GLuint shader = glCreateShader(shaderType);
      
      // Attch the shader source and then compile
      const char* shaderString = shaderSrc.c_str();
      glShaderSource(shader, 1, &shaderString, nullptr);
      
      // Compile the shader
      glCompileShader(shader);
      
      // Shader Error Handling
      GLint isCompiled = 0;
      glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
        glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
        
        // Delete the shader if error pops
        glDeleteShader(shader);
        
        IK_LOG_ERROR(LogModule::Shader, "{0}", infoLog.data());
        IK_ASSERT(false, "Shader compilation failure!");
      } // Error Check for shader Compiler
      
      // Attach both shader and link them if compiled successfully
      glAttachShader(m_rendererID, shader);
      shaderIDs.push_back(shader);
      
      SHADER_LOG("    Compiled {0} shader ", ShaderUtils::ShaderNameFromType(shaderType));
    }
    
    // Shader Linking
    glLinkProgram(m_rendererID);
    
    // Shader Error Handling. Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLinked = 0;
    glGetProgramiv(m_rendererID, GL_LINK_STATUS, static_cast<int32_t*>(&isLinked));
    
    if (isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);
      
      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
      glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);
      
      // We don't need the program anymore.
      glDeleteProgram(m_rendererID);
      
      for (auto id : shaderIDs)
      {
        glDeleteShader(id);
      }
      
      IK_LOG_ERROR(LogModule::Shader, "{0}", infoLog.data());
      IK_ASSERT(false, "Shader link failure!");
    } // Error check of Shader Linker
    
    // Delete all shader as we have already linked them to our shader program
    for (auto id : shaderIDs)
    {
      glDeleteShader(id);
    }
  }
} // namespace IKan
