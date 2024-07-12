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
      
      // Compile and Link the shader code
      Compile();
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
  
  void OpenGLShader::Compile()
  {
    IK_PROFILE();
    SHADER_LOG("  Compiling {0} ", m_name.c_str());
    
    // Check Mendatory Shaders : Vertex and Fragment are present
    IK_ASSERT(m_shaderSourceCodeMap.find(GL_VERTEX_SHADER) != m_shaderSourceCodeMap.end(), "Vertex Shader not loaded in file");
    IK_ASSERT(m_shaderSourceCodeMap.find(GL_FRAGMENT_SHADER) != m_shaderSourceCodeMap.end(), "Fragment Shader not loaded in file");
    
    // Stores the shader code IDs
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
      if (GL_FALSE == isCompiled)
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
    }
    
    // Shader Linking
    glLinkProgram(m_rendererID);
    
    // Shader Error Handling. Note the different functions here: glGetProgram* instead of glGetShader
    GLint isLinked = 0;
    glGetProgramiv(m_rendererID, GL_LINK_STATUS, static_cast<int32_t*>(&isLinked));
    
    if (GL_FALSE == isLinked)
    {
      GLint maxLength = 0;
      glGetProgramiv(m_rendererID, GL_INFO_LOG_LENGTH, &maxLength);
      
      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(static_cast<size_t>(maxLength));
      glGetProgramInfoLog(m_rendererID, maxLength, &maxLength, &infoLog[0]);
      
      // We don't need the program anymore.
      glDeleteProgram(m_rendererID);
      
      for (GLuint id : shaderIDs)
      {
        glDeleteShader(id);
      }
      
      IK_LOG_ERROR(LogModule::Shader, "{0}", infoLog.data());
      IK_ASSERT(false, "Shader link failure!");
    } // Error check of Shader Linker
    
    // Delete all shader as we have already linked them to our shader program
    for (GLuint id : shaderIDs)
    {
      glDeleteShader(id);
    }
  }
} // namespace IKan
