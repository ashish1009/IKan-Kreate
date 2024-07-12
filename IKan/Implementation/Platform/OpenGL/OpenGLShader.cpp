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
    
    /// This function returns Open GL shader domain typy from internal type
    /// - Parameter domain: internal domain type
    static ShaderDomain GlDomainToShaderDomain(const GLint domain)
    {
      if (domain == GL_VERTEX_SHADER)         return ShaderDomain::Vertex;
      else if (domain == GL_FRAGMENT_SHADER)  return ShaderDomain::Fragment;
      else if (domain == GL_GEOMETRY_SHADER)  return ShaderDomain::Geometry;
      else IK_ASSERT(false, "Invalid domain!");
      return ShaderDomain::None;
    }
    /// This function returns the Open GL Shader Name from Shader domain
    /// - Parameter type: type of shader in GL enum
    [[maybe_unused]] static const char* ShaderNameFromType(ShaderDomain type)
    {
      switch (type)
      {
        case ShaderDomain::Vertex: return "Vertex";
        case ShaderDomain::Geometry: return "Geometry";
        case ShaderDomain::Fragment: return "Fragment";
        case ShaderDomain::None:
        default:
          IK_ASSERT(false, "Invalid domain!");
      }
      return "Invalid";
    }
    /// This function returns the Open GL Shader Name from Open GL Type
    /// - Parameter type: type of shader in GL enum
    [[maybe_unused]] static const char* ShaderNameFromType(const GLenum type)
    {
      if (type == GL_VERTEX_SHADER) return "Vertex";
      if (type == GL_FRAGMENT_SHADER) return "Fragment";
      if (type == GL_GEOMETRY_SHADER) return "Geomatry";
      IK_ASSERT(false, "Unknown shader type!");
      return "Invalid";
    }
    /// This function returns true if type is of resource
    /// - Parameter type: type of field
    [[maybe_unused]] static bool IsTypeStringResource(const std::string& type)
    {
      if (type == "sampler2D")          return true;
      if (type == "sampler2DMS")        return true;
      if (type == "samplerCube")        return true;
      if (type == "sampler2DShadow")    return true;
      return false;
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
      
      // Parse shader and Store all the structures and uniforms in Shader class
      Parse();
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
  
  void OpenGLShader::Parse()
  {
    IK_PROFILE();
    // Holds the pointer of tokens either "struct" or "uniform"
    const char* token;

    for (auto& [domain, shaderCode] : m_shaderSourceCodeMap)
    {
      // Parsing the structure
      const char* str = shaderCode.c_str();
      while ((token = Utils::String::FindToken(str, "struct")))
      {
        GLint glDomain = static_cast<GLint>(domain);
        ParseUniformStruct(Utils::String::GetBlock(token, &str), ShaderUtils::GlDomainToShaderDomain(glDomain));
      }
      
      // Parsing uniforms
      str = shaderCode.c_str();
      SHADER_LOG("  Parsing the '{0}' to extracts all the Uniforms for '{1}' Shader", m_name, ShaderUtils::ShaderNameFromType(domain));
      SHADER_LOG("    Parsing the Uniforms: ");
      while ((token = Utils::String::FindToken(str, "uniform")))
      {
        ParseUniform(Utils::String::GetStatement(token, &str), ShaderUtils::GlDomainToShaderDomain((GLint)domain));
      }
    }
  }
  
  void OpenGLShader::ParseUniformStruct(const std::string &block, ShaderDomain domain)
  {
    IK_PROFILE();
    
    // Log the parsing structure only once
    if (0 == m_structs.size())
    {
      SHADER_LOG("  Parsing the '{0}' to extracts all the structures for '{1}' shader", m_name, ShaderUtils::ShaderNameFromType(domain));
    }
    
    // Get each word from the block and store them in vector
    std::vector<std::string> tokens = Utils::String::Tokenize(block);
    uint32_t index = 1; // 0 is for keyword "struct"

    // Get the name of structure
    std::string structName = tokens[index++];
    ShaderStruct* uniformStruct = new ShaderStruct(structName);
    index++;  // 2 is for '{'

    SHADER_LOG("    struct {0} ", structName);

    // Parse the strcuture
    while (index < tokens.size())
    {
      // If one block of structure ends
      if (tokens[index] == "}")
      {
        break;
      }
      
      std::string fieldType = tokens[index++]; // Type of element
      std::string fieldName = tokens[index++]; // Name of element

      // Strip ';' from name if present
      if (const char* s = strstr(fieldName.c_str(), ";"))
      {
        size_t sizeOfFieldName = (size_t)(s - fieldName.c_str());
        fieldName = std::string(fieldName.c_str(), sizeOfFieldName);
      }

      // Check is it array if yes the extract count
      uint32_t count = 1;
      const char* nameStr = fieldName.c_str();
      if (const char* countWithBrackets = strstr(nameStr, "["))
      {
        std::string nameWithoutCount = std::string(nameStr, (size_t)(countWithBrackets - nameStr));
        const char* end = strstr(nameStr, "]");
        std::string countWithLastBracket(countWithBrackets + 1, (size_t)(end - countWithBrackets));

        count = (uint32_t)atoi(countWithLastBracket.c_str());
        fieldName = countWithLastBracket;
      } // if (const char* count_with_brackets = strstr(name_str, "["))

      // Stores the content of structure in struct
      ShaderUniformDeclaration* field = new OpenGLShaderUniformDeclaration(domain, OpenGLShaderUniformDeclaration::StringToType(fieldType), fieldName, count);
      uniformStruct->AddField(field);
    }
    m_structs.emplace_back(uniformStruct);
  }
  
  void OpenGLShader::ParseUniform(const std::string &statement, ShaderDomain domain)
  {
    IK_PROFILE();
    // Store all the statements after keyword uniform
    std::vector<std::string> tokens = Utils::String::Tokenize(statement);
    uint32_t index = 1; // 0th is for keyword unifrom
    
    std::string fieldType = tokens[index++];
    std::string fieldName = tokens[index++];
    
    // Strip ; from name if present
    if (const char* s = strstr(fieldName.c_str(), ";"))
    {
      fieldName = std::string(fieldName.c_str(), (size_t)(s - fieldName.c_str()));
    }
    
    std::string nameWithSize(fieldName);
    
    // Check is it array if yes the extract count
    uint32_t count = 1;
    const char* nameStr = nameWithSize.c_str();
    if (const char* countWithBrackets = strstr(nameStr, "["))
    {
      std::string nameWithoutCount = std::string(nameStr, (size_t)(countWithBrackets - nameStr));
      const char* end = strstr(nameStr, "]");
      std::string countWithLastBracket(countWithBrackets + 1, (size_t)(end - countWithBrackets));
      
      count = (uint32_t)atoi(countWithLastBracket.c_str());
      fieldName = nameWithoutCount;
    } // if (const char* count_with_brackets = strstr(name_str, "["))
    
    // Store the resources uniform inside shader
    if (ShaderUtils::IsTypeStringResource(fieldType))
    {
      // Resources like Sampler 2D
      ShaderResourceDeclaration* declaration = new OpenGLShaderResourceDeclaration(OpenGLShaderResourceDeclaration::StringToType(fieldType), fieldName, count);
      m_resources.push_back(declaration);
    }
    else
    {
      // if field is not of type resources like sampler
      // Resources fundamental and structures
      OpenGLShaderUniformDeclaration::Type type = OpenGLShaderUniformDeclaration::StringToType(fieldType);
      OpenGLShaderUniformDeclaration* declaration = nullptr;
      
      if (OpenGLShaderUniformDeclaration::Type::None == type)
      {
        // Find struct
        // NOTE: in this case field type is the name of struct as we write like
        // this " uniform < field type     >  < field name   > "
        //      " uniform < name of struct >  < uniform name > "
        ShaderStruct* structure = FindStruct(fieldType);
        IK_ASSERT(structure, "");
        declaration = new OpenGLShaderUniformDeclaration(domain, structure, fieldName, count);
      }
      else
      {
        declaration = new OpenGLShaderUniformDeclaration(domain, type, fieldName, count);
      }
      
      // Store all the uniforms in buffers
      if (ShaderDomain::Vertex == domain)
      {
        if (!m_vsMaterialUniformBuffer)
        {
          m_vsMaterialUniformBuffer.reset(new OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_vsMaterialUniformBuffer->PushUniform(declaration);
      }
      else if (ShaderDomain::Fragment == domain)
      {
        if (!m_fsMaterialUniformBuffer)
        {
          m_fsMaterialUniformBuffer.reset(new OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_fsMaterialUniformBuffer->PushUniform(declaration);
      }
      else if (ShaderDomain::Geometry == domain)
      {
        if (!m_gsMaterialUniformBuffer)
        {
          m_gsMaterialUniformBuffer.reset(new OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_gsMaterialUniformBuffer->PushUniform(declaration);
      }
    }
  }
  
  ShaderStruct* OpenGLShader::FindStruct(std::string_view name)
  {
    IK_PROFILE();
    for (ShaderStruct* s : m_structs)
    {
      if (s->GetName() == name)
      {
        return s;
      }
    }
    return nullptr;
  }
} // namespace IKan
