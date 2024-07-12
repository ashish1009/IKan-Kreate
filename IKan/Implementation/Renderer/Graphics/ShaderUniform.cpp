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
} // namespace IKan
