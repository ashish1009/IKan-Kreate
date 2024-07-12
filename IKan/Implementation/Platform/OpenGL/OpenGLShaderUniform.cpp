//
//  OpenGLShaderUniform.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLShaderUniform.hpp"

namespace IKan
{
#define SHADER_LOG(...) IK_LOG_DEBUG(LogModule::Shader, __VA_ARGS__);

  // OpenGLShaderUniformDeclaration ----------------------------------------------------------------------------------
  OpenGLShaderUniformDeclaration::OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count)
  : m_struct(nullptr), m_type(type), m_domain(domain), m_name(name), m_count(count)
  {
    m_size = SizeOfUniformType(m_type) * m_count;
    SHADER_LOG("      {0} {1}[{2}] (Size : {3}) ", TypeToString(m_type), m_name, m_count, m_size);
  }

  OpenGLShaderUniformDeclaration::~OpenGLShaderUniformDeclaration()
  {
    SHADER_LOG("      Destroying : {0} {1}[{2}] (Size : {3}) ", TypeToString(m_type), m_name, m_count, m_size);
  }

  void OpenGLShaderUniformDeclaration::SetOffset(uint32_t offset)
  {
    if (m_type == OpenGLShaderUniformDeclaration::Type::Struct)
    {
      m_struct->SetOffset(offset);
    }
    m_offset = offset;
  }
  
  const std::string& OpenGLShaderUniformDeclaration::GetName() const
  {
    return m_name;
  }
  uint32_t OpenGLShaderUniformDeclaration::GetSize() const
  {
    return m_size;
  }
  uint32_t OpenGLShaderUniformDeclaration::GetCount() const
  {
    return m_count;
  }
  uint32_t OpenGLShaderUniformDeclaration::GetOffset() const
  {
    return m_offset;
  }
  ShaderDomain OpenGLShaderUniformDeclaration::GetDomain() const
  {
    return m_domain;
  }
  OpenGLShaderUniformDeclaration::Type OpenGLShaderUniformDeclaration::GetType() const
  {
    return m_type;
  }
  bool OpenGLShaderUniformDeclaration::IsArray() const
  {
    return m_count > 1;
  }
  uint32_t OpenGLShaderUniformDeclaration::GetAbsoluteOffset() const
  {
    return m_struct ? m_struct->GetOffset() + m_offset : m_offset;
  }
  int32_t OpenGLShaderUniformDeclaration::GetLocation(uint32_t index) const
  {
    return m_locations[index];
  }
  const ShaderStruct& OpenGLShaderUniformDeclaration::GetShaderUniformStruct() const
  {
    IK_ASSERT(m_struct, "Struct is NULL!");
    return *m_struct;
  }
  
  uint32_t OpenGLShaderUniformDeclaration::SizeOfUniformType(Type type)
  {
    switch (type)
    {
      case OpenGLShaderUniformDeclaration::Type::None:    return 0;
      case OpenGLShaderUniformDeclaration::Type::Bool:
      case OpenGLShaderUniformDeclaration::Type::Int32:
      case OpenGLShaderUniformDeclaration::Type::Float32: return 4;
      case OpenGLShaderUniformDeclaration::Type::Vec2:    return 4 * 2;
      case OpenGLShaderUniformDeclaration::Type::Vec3:    return 4 * 3;
      case OpenGLShaderUniformDeclaration::Type::Vec4:    return 4 * 4;
      case OpenGLShaderUniformDeclaration::Type::Mat3:    return 4 * 3 * 3;
      case OpenGLShaderUniformDeclaration::Type::Mat4:    return 4 * 4 * 4;
      case OpenGLShaderUniformDeclaration::Type::Struct:
      default:
        IK_ASSERT(false, "No need to return the size in case of struct!")
        break;
    }
    return 0;
  }
  
  OpenGLShaderUniformDeclaration::Type OpenGLShaderUniformDeclaration::StringToType(const std::string& type)
  {
    if (type == "bool")     return Type::Bool;
    if (type == "int")      return Type::Int32;
    if (type == "float")    return Type::Float32;
    if (type == "vec2")     return Type::Vec2;
    if (type == "vec3")     return Type::Vec3;
    if (type == "vec4")     return Type::Vec4;
    if (type == "mat3")     return Type::Mat3;
    if (type == "mat4")     return Type::Mat4;
    
    return Type::None;
  }
  
  std::string OpenGLShaderUniformDeclaration::TypeToString(Type type)
  {
    switch (type)
    {
      case OpenGLShaderUniformDeclaration::Type::None:       return "None  ";
      case OpenGLShaderUniformDeclaration::Type::Bool:       return "bool  ";
      case OpenGLShaderUniformDeclaration::Type::Int32:      return "int32 ";
      case OpenGLShaderUniformDeclaration::Type::Float32:    return "float ";
      case OpenGLShaderUniformDeclaration::Type::Vec2:       return "vec2  ";
      case OpenGLShaderUniformDeclaration::Type::Vec3:       return "vec3  ";
      case OpenGLShaderUniformDeclaration::Type::Vec4:       return "vec4  ";
      case OpenGLShaderUniformDeclaration::Type::Mat3:       return "mat3  ";
      case OpenGLShaderUniformDeclaration::Type::Mat4:       return "mat4  ";
      case OpenGLShaderUniformDeclaration::Type::Struct:     return "struct";
    }
    return "Invalid Type";
  }
  

} // namespace IKan
