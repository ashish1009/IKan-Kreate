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
    /// This function returns Open GL shader domain typy from internal type
    /// - Parameter domain: internal domain type
    static ShaderDomain GlDomainToShaderDomain(GLint domain)
    {
      if (domain == GL_VERTEX_SHADER)         return ShaderDomain::Vertex;
      else if (domain == GL_FRAGMENT_SHADER)  return ShaderDomain::Fragment;
      else if (domain == GL_GEOMETRY_SHADER)  return ShaderDomain::Geometry;
      else IK_ASSERT(false, "Invalid domain");
    }
    /// This function returns true if type is of resource
    /// - Parameter type: type of field
    static bool IsTypeStringResource(const std::string& type)
    {
      if (type == "sampler2D")          return true;
      if (type == "sampler2DMS")        return true;
      if (type == "samplerCube")        return true;
      if (type == "sampler2DShadow")    return true;
      return false;
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
    /// This function returns the Open GL Shader Name from Open GL Type
    /// - Parameter type: type of shader in GL enum
    [[maybe_unused]] static const char* ShaderNameFromType(ShaderDomain type)
    {
      switch (type)
      {
        case ShaderDomain::Vertex: return "Vertex";
        case ShaderDomain::Geometry: return "Geometry";
        case ShaderDomain::Fragment: return "Fragment";
        case ShaderDomain::None:
        default: IK_ASSERT(false, "Invalid domain");
      }
    }
    /// This funciton returns the Open GL Shader Name from Type
    /// - Parameter type: type of shader in GL enum
    [[maybe_unused]] static const char* ShaderNameFromInternalType(ShaderDomain type)
    {
      switch (type)
      {
        case ShaderDomain::Vertex: return "Vertex";
        case ShaderDomain::Geometry: return "Geometry";
        case ShaderDomain::Fragment: return "Fragment";
        case ShaderDomain::None:
        default: IK_ASSERT(false, "Invalid domain");
      }
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

    // Parse shader and Store all the structures and uniforms in Shader class
    Parse();
    
    // Resolve the location of all uniforms
    ResolveUniforms();
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
  
  void OpenGLShader::Parse()
  {
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
    // Log the parsing structure only once
    if (m_structs.size() == 0) {
      SHADER_LOG("  Parsing the '{0}' to extracts all the structures for '{1}' shader",
                 m_name, ShaderUtils::ShaderNameFromType(domain));
    }
    
    // Get each word from the block and store them in vector
    std::vector<std::string> tokens = Utils::String::Tokenize(block);
    uint32_t index = 1; // 0 is for keyword "struct"
    
    // Get the name of structure
    std::string structName = tokens[index++];
    ShaderStruct* uniformStruct = iknew ShaderStruct(structName);
    index++;  // 1 is for Name (stored)
    
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
        auto nameWithoutCount = std::string(nameStr, (size_t)(countWithBrackets - nameStr));
        const char* end = strstr(nameStr, "]");
        std::string countWithLastBracket(countWithBrackets + 1, (size_t)(end - countWithBrackets));
        
        count = (uint32_t)atoi(countWithLastBracket.c_str());
        fieldName = countWithLastBracket;
      } // if (const char* count_with_brackets = strstr(name_str, "["))
      
      // Stores the content of structure in struct
      ShaderUniformDeclaration* field = iknew OpenGLShaderUniformDeclaration(domain,
                                                                             OpenGLShaderUniformDeclaration::StringToType(fieldType),
                                                                             fieldName, count);
      uniformStruct->AddField(field);
    }
    m_structs.emplace_back(uniformStruct);
  }
  
  void OpenGLShader::ParseUniform(const std::string &statement, ShaderDomain domain)
  {
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
      auto nameWithoutCount = std::string(nameStr, (size_t)(countWithBrackets - nameStr));
      const char* end = strstr(nameStr, "]");
      std::string countWithLastBracket(countWithBrackets + 1, (size_t)(end - countWithBrackets));
      
      count = (uint32_t)atoi(countWithLastBracket.c_str());
      fieldName = nameWithoutCount;
    } // if (const char* count_with_brackets = strstr(name_str, "["))
    
    // Store the resources uniform inside shader
    if (ShaderUtils::IsTypeStringResource(fieldType))
    {
      // Resources like Sampler 2D
      ShaderResourceDeclaration*
      declaration = iknew OpenGLShaderResourceDeclaration(OpenGLShaderResourceDeclaration::StringToType(fieldType),
                                                          fieldName, count);
      m_resources.push_back(declaration);
    }
    else
    {
      // if field is not of type resources like sampler
      // Resources fundamental and structures
      OpenGLShaderUniformDeclaration::Type type = OpenGLShaderUniformDeclaration::StringToType(fieldType);
      OpenGLShaderUniformDeclaration* declaration = nullptr;
      
      if (type == OpenGLShaderUniformDeclaration::Type::None)
      {
        // Find struct
        // NOTE: in this case field type is the name of struct as we write like
        // this " uniform < field type     >  < field name   > "
        //      " uniform < name of struct >  < uniform name > "
        ShaderStruct* structure = FindStruct(fieldType);
        IK_ASSERT(structure, "");
        declaration = iknew OpenGLShaderUniformDeclaration(domain, structure, fieldName, count);
      }
      else
      {
        // if (type == OpenGLShaderUniformDeclaration::Type::kStruct)
        declaration = iknew OpenGLShaderUniformDeclaration(domain, type, fieldName, count);
      }
      
      // Store all the uniforms in buffers
      if (domain == ShaderDomain::Vertex)
      {
        if (!m_vsMaterialUniformBuffer)
        {
          m_vsMaterialUniformBuffer.reset(iknew OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_vsMaterialUniformBuffer->PushUniform(declaration);
      }
      else if (domain == ShaderDomain::Fragment)
      {
        if (!m_fsMaterialUniformBuffer)
        {
          m_fsMaterialUniformBuffer.reset(iknew OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_fsMaterialUniformBuffer->PushUniform(declaration);
      }
      else if (domain == ShaderDomain::Geometry)
      {
        if (!m_gsMaterialUniformBuffer)
        {
          m_gsMaterialUniformBuffer.reset(iknew OpenGLShaderUniformBufferDeclaration("", domain));
        }
        m_gsMaterialUniformBuffer->PushUniform(declaration);
      }
    }
  }
  
  ShaderStruct* OpenGLShader::FindStruct(const std::string& name)
  {
    for (ShaderStruct* s : m_structs)
    {
      if (s->GetName() == name)
      {
        return s;
      }
    }
    return nullptr;
  }

  void OpenGLShader::ResolveUniforms()
  {
    SHADER_LOG("  Resolving Uniform locations for '{0}'", m_name);
    
    // Uniform samplers for textures, cubemaps etc
    SHADER_LOG("    Resolving Uniforms for Samplers...");
    // Setting location of sampler uniform
    uint32_t sampler = 0;
    for (size_t i = 0; i < m_resources.size(); i++)
    {
      OpenGLShaderResourceDeclaration* resource = (OpenGLShaderResourceDeclaration*)m_resources[i];
      int32_t location = GetUniformLocation(resource->m_name);
      
      // For single samplers
      if (resource->GetCount() == 1)
      {
        resource->m_register = sampler;
        if (location != -1)
        {
          SHADER_LOG("      Location : {0} for {1}[{2}]", sampler, resource->m_name, resource->GetCount());
          SetUniformInt1(resource->m_name, (int32_t)sampler);
        }
        sampler++;
      }
      // For arrya of samplers
      else if (resource->GetCount() > 1)
      {
        resource->m_register = 0;
        
        uint32_t count = resource->GetCount();
        int32_t* samplers = iknew int32_t[count];
        
        for (uint32_t s = 0; s < count; s++)
        {
          samplers[s] = (int32_t)s;
        }
        SHADER_LOG("      Location : {0} to {1} for {2}[{3}]", 0, count, resource->GetName(), resource->GetCount());
        SetIntArray(resource->GetName(), samplers, count);
        ikdelete[] samplers;
      }
    } // for (size_t i = 0; i < resources_.size(); i++)
    
    // Unifrom resources of structors or fundamentals
    std::shared_ptr<OpenGLShaderUniformBufferDeclaration> decls[3] =
    {
      m_vsMaterialUniformBuffer,
      m_fsMaterialUniformBuffer,
      m_gsMaterialUniformBuffer
    };
    
    for (uint8_t shaderIdx = 0; shaderIdx < MaxShaderSupported; shaderIdx++)
    {
      auto decl = decls[shaderIdx];
      if (!decl)
      {
        continue;
      }
      
      SHADER_LOG("    Resolving Uniforms for Datatypes of '{0}' Shader...",
                 ShaderUtils::ShaderNameFromInternalType((ShaderDomain)(shaderIdx + 1)));
      
      const std::vector<ShaderUniformDeclaration*>& uniforms = decl->GetUniformDeclarations();
      for (size_t j = 0; j < uniforms.size(); j++)
      {
        OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[j];
        
        // Uniform Structures
        if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::Struct)
        {
          const ShaderStruct& s = uniform->GetShaderUniformStruct();
          const auto& fields = s.GetFields();
          
          // If Array of Structure
          if (uniform->GetCount() > 1)
          {
            for (size_t l = 0; l < uniform->GetCount(); l++)
            {
              for (size_t k = 0; k < fields.size(); k++)
              {
                OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
                std::string uniformName = uniform->m_name + "[" + std::to_string(l) + "]." + field->m_name;
                uint32_t location = (uint32_t)GetUniformLocation(uniformName);
                field->m_locations.emplace_back(location);
                SHADER_LOG("      Location : {0} for {1}[{3}].{2}", location, s.GetName(), field->GetName(), l);
              } // for (size_t k = 0; k < fields.size(); k++)
            } // for (size_t l = 0; l < uniform->GetCount(); l++)
          }
          else
          { // if (uniform->GetCount() > 1)
            // Single struct uniform
            for (size_t k = 0; k < fields.size(); k++)
            {
              OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
              uint32_t location = (uint32_t)GetUniformLocation(uniform->m_name + "." + field->m_name);
              field->m_locations.emplace_back(location);
              SHADER_LOG("      Location : {0} for {1}.{2} [{3}]", location, s.GetName(), field->GetName(), field->GetCount());
            }
          } // else : if (uniform->GetCount() > 1)
        }
        else
        { // if (uniform->GetType() == OpenGLShaderUniformDeclaration::Type::kStruct)
          // Fundamental uniforms
          uint32_t location = (uint32_t)GetUniformLocation(uniform->m_name);
          uniform->m_locations.emplace_back(location);
          SHADER_LOG("      Location : {0} for {1}[{2}]", location, uniform->GetName(), uniform->GetCount());
        }
      } // for (size_t j = 0; j < uniforms.size(); j++)
    } // for (uint8_t shaderIdx = 0; shaderIdx < MaxShaderSupported; shaderIdx++)
  }
  
  int32_t OpenGLShader::GetUniformLocation(const std::string& name)
  {
    if (m_locationMap.find(name) != m_locationMap.end())
    {
      return m_locationMap.at(name);
    }
    
    int32_t location = glGetUniformLocation(m_rendererID, name.c_str());
    if (-1 == location)
    {
      SHADER_LOG("Warning: uniform '{0}' doesnt exist", name);
    }
    m_locationMap[name] = location;
    return location;
  }

  void OpenGLShader::SetVSMaterialUniformBuffer(const Buffer& buffer)
  {
    glUseProgram(m_rendererID);
    ResolveAndSetUniforms(m_vsMaterialUniformBuffer, buffer);
  }
  
  void OpenGLShader::SetFSMaterialUniformBuffer(const Buffer& buffer)
  {
    glUseProgram(m_rendererID);
    ResolveAndSetUniforms(m_fsMaterialUniformBuffer, buffer);
  }
  
  void OpenGLShader::SetGSMaterialUniformBuffer(const Buffer& buffer)
  {
    glUseProgram(m_rendererID);
    ResolveAndSetUniforms(m_gsMaterialUniformBuffer, buffer);
  }
  
  void OpenGLShader::ResolveAndSetUniforms(const Ref<OpenGLShaderUniformBufferDeclaration>& decl, const Buffer& buffer)
  {
    const std::vector<ShaderUniformDeclaration*>& uniforms = decl->GetUniformDeclarations();
    for (size_t i = 0; i < uniforms.size(); i++)
    {
      OpenGLShaderUniformDeclaration* uniform = (OpenGLShaderUniformDeclaration*)uniforms[i];
      if (uniform->IsArray())
      {
        ResolveAndSetUniformArray(uniform, buffer);
      }
      else
      {
        ResolveAndSetUniform(uniform, buffer);
      }
    }
  }
  
  void OpenGLShader::ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer)
  {
    // FIXME: (IKan) Add Check of location later
    uint32_t offset = uniform->GetOffset();
    switch (uniform->GetType())
    {
      case OpenGLShaderUniformDeclaration::Type::Float32:
        UploadUniformFloat1(uniform->GetLocation(), *(float*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Int32:
      case OpenGLShaderUniformDeclaration::Type::Bool:
        UploadUniformInt1(uniform->GetLocation(), *(int32_t*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec2:
        UploadUniformFloat2(uniform->GetLocation(), *(glm::vec2*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec3:
        UploadUniformFloat3(uniform->GetLocation(), *(glm::vec3*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec4:
        UploadUniformFloat4(uniform->GetLocation(), *(glm::vec4*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat3:
        UploadUniformMat3(uniform->GetLocation(), *(glm::mat3*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat4:
        UploadUniformMat4(uniform->GetLocation(), *(glm::mat4*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Struct:
        UploadUniformStruct(uniform, buffer.data, offset);
        break;
      default:
        IK_ASSERT(false, "Unknown uniform type!");
    }
  }
  
  void OpenGLShader::ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, const Buffer& buffer)
  {
    // FIXME: (IKan) Add Check of location later
    uint32_t offset = uniform->GetOffset();
    switch (uniform->GetType())
    {
      case OpenGLShaderUniformDeclaration::Type::Float32:
        UploadUniformFloat1(uniform->GetLocation(), *(float*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Int32:
      case OpenGLShaderUniformDeclaration::Type::Bool:
        UploadUniformInt1(uniform->GetLocation(), *(int32_t*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec2:
        UploadUniformFloat2(uniform->GetLocation(), *(glm::vec2*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec3:
        UploadUniformFloat3(uniform->GetLocation(), *(glm::vec3*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec4:
        UploadUniformFloat4(uniform->GetLocation(), *(glm::vec4*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat3:
        UploadUniformMat3(uniform->GetLocation(), *(glm::mat3*)&buffer.data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat4:
        UploadUniformMat4Array((uint32_t)uniform->GetLocation(), *(glm::mat4*)&buffer.data[offset], uniform->GetCount());
        break;
      case OpenGLShaderUniformDeclaration::Type::Struct:
        UploadUniformStruct(uniform, buffer.data, offset);
        break;
      default:
        IK_ASSERT(false, "Unknown uniform type!");
    }
  }
  
  void OpenGLShader::ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, std::byte* data, int32_t offset, uint8_t idx)
  {
    int32_t location = field.m_locations[idx];
    if (location == -1)
    {
      return;
    }
    
    switch (field.GetType())
    {
      case OpenGLShaderUniformDeclaration::Type::Float32:
        UploadUniformFloat1(location, *(float*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Int32:
      case OpenGLShaderUniformDeclaration::Type::Bool:
        UploadUniformInt1(location, *(int32_t*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec2:
        UploadUniformFloat2(location, *(glm::vec2*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec3:
        UploadUniformFloat3(location, *(glm::vec3*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Vec4:
        UploadUniformFloat4(location, *(glm::vec4*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat3:
        UploadUniformMat3(location, *(glm::mat3*)&data[offset]);
        break;
      case OpenGLShaderUniformDeclaration::Type::Mat4:
        UploadUniformMat4(location, *(glm::mat4*)&data[offset]);
        break;
      default:
        IK_ASSERT(false, "Unknown uniform type!");
    }
  }
  
  void OpenGLShader::UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, std::byte* buffer, uint32_t offset)
  {
    const ShaderStruct& s = uniform->GetShaderUniformStruct();
    const auto& fields = s.GetFields();
    
    for (uint8_t i = 0; i < uniform->GetCount(); i++)
    {
      for (size_t k = 0; k < fields.size(); k++)
      {
        OpenGLShaderUniformDeclaration* field = (OpenGLShaderUniformDeclaration*)fields[k];
        ResolveAndSetUniformField(*field, buffer, (int32_t)offset, i);
        offset += field->m_size;
      }
    }
  }
  
  void OpenGLShader::Bind() const
  {
    glUseProgram(m_rendererID);
  }
  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  };
  
  const std::string& OpenGLShader::GetName() const
  {
    return m_name;
  }
  const std::string& OpenGLShader::GetFilePath() const
  {
    return m_filePath;
  }
  RendererID OpenGLShader::GetRendererID() const
  {
    return m_rendererID;
  }
  
  bool OpenGLShader::HasVSMaterialUniformBuffer() const
  {
    return (bool)m_vsMaterialUniformBuffer;
  }
  bool OpenGLShader::HasFSMaterialUniformBuffer() const
  {
    return (bool)m_fsMaterialUniformBuffer;
  }
  bool OpenGLShader::HasGSMaterialUniformBuffer() const
  {
    return (bool)m_gsMaterialUniformBuffer;
  }
  
  const ShaderUniformBufferDeclaration& OpenGLShader::GetVSMaterialUniformBuffer() const
  {
    return *m_vsMaterialUniformBuffer;
  }
  const ShaderUniformBufferDeclaration& OpenGLShader::GetFSMaterialUniformBuffer() const
  {
    return *m_fsMaterialUniformBuffer;
  }
  const ShaderUniformBufferDeclaration& OpenGLShader::GetGSMaterialUniformBuffer() const
  {
    return *m_gsMaterialUniformBuffer;
  }
  
  const std::vector<ShaderResourceDeclaration*>& OpenGLShader::GetResources() const
  {
    return m_resources;
  }
  
  // Uniforms with name ----------------------------------------------------------------------------------------------
  void OpenGLShader::SetUniformInt1(const std::string& name, int32_t value)
  {
    glUseProgram(m_rendererID);
    glUniform1i(GetUniformLocation(name), value);
  }
  
  void OpenGLShader::SetIntArray(const std::string& name, int32_t* values, uint32_t count)
  {
    glUseProgram(m_rendererID);
    int32_t* textureArraySlotData = iknew int32_t[count];
    memcpy(textureArraySlotData, values, count * sizeof(int32_t));
    
    glUniform1iv(GetUniformLocation(name), (GLsizei)count, textureArraySlotData);
    ikdelete[] textureArraySlotData;
  }
  void OpenGLShader::SetUniformMat4Array(const std::string& name, const glm::mat4& values, uint32_t count)
  {
    glUniformMatrix4fv(GetUniformLocation(name), (GLsizei)count, GL_FALSE, glm::value_ptr(values));
  }
  
  void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value)
  {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
  }
  
  void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& value)
  {
    glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
  }
  
  void OpenGLShader::SetUniformFloat1(const std::string& name, float value)
  {
    glUniform1f(GetUniformLocation(name), value);
  }
  
  void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
  {
    glUniform2f(GetUniformLocation(name), value.x, value.y);
  }
  
  void OpenGLShader::SetUniformFloat3(const  std::string& name, const glm::vec3& value)
  {
    glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
  }
  
  void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
  {
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
  }
  
  // Uniforms with location -----------------------------------------------------------------------------------------
  void OpenGLShader::UploadUniformInt1(int32_t location, int32_t value)
  {
    glUniform1i(location, value);
  }
  
  void OpenGLShader::UploadUniformMat4(int32_t location, const glm::mat4& value)
  {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }
  
  void OpenGLShader::UploadUniformMat3(int32_t location, const glm::mat3& value)
  {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
  }
  
  void OpenGLShader::UploadUniformFloat1(int32_t location, float value)
  {
    glUniform1f(location, value);
  }
  
  void OpenGLShader::UploadUniformFloat2(int32_t location, const glm::vec2& value)
  {
    glUniform2f(location, value.x, value.y);
  }
  
  void OpenGLShader::UploadUniformFloat3(int32_t location, const glm::vec3& value)
  {
    glUniform3f(location, value.x, value.y, value.z);
  }
  
  void OpenGLShader::UploadUniformFloat4(int32_t location, const glm::vec4& value)
  {
    glUniform4f(location, value.x, value.y, value.z, value.w);
  }
  
  void OpenGLShader::UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count)
  {
    glUniformMatrix4fv((GLint)location, (GLsizei)count, GL_FALSE, glm::value_ptr(values));
  }} // namespace IKan
