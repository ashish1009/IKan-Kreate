//
//  ShaderUniform.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "ShaderUniform.hpp"

namespace IKan
{
  ShaderStruct::ShaderStruct(std::string_view name)
  : m_name(name.data()), m_size(0), m_offset(0)
  {
    
  }
  ShaderStruct::~ShaderStruct()
  {
    for (ShaderUniformDeclaration* field : m_fields)
    {
      delete field;
    }
  }
  
  void ShaderStruct::SetOffset(uint32_t offset)
  {
    m_offset = offset;
  }
  
  uint32_t ShaderStruct::GetSize() const
  {
    return m_size;
  }
  uint32_t ShaderStruct::GetOffset() const
  {
    return m_offset;
  }
  const std::string& ShaderStruct::GetName() const
  {
    return m_name;
  }
  const std::vector<ShaderUniformDeclaration*>& ShaderStruct::GetFields() const
  {
    return m_fields;
  }
  void ShaderStruct::AddField(ShaderUniformDeclaration* field)
  {
    IK_PROFILE();
    m_size += field->GetSize();
    
    uint32_t offset = 0;
    if (m_fields.size())
    {
      ShaderUniformDeclaration* previous = m_fields.back();
      offset = previous->GetOffset() + previous->GetSize();
    }
    
    field->SetOffset(offset);
    m_fields.push_back(field);
  }
} // namespace IKan
