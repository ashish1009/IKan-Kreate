//
//  ShaderUniform.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

namespace IKan
{
  /// This enum stores the type of shader domain
  enum class ShaderDomain : uint8_t
  {
    None = 0,
    Vertex = 1,
    Fragment = 2,
    Geometry = 3
  };
  
  /// This class is the interface to store shader uniform elements (like int, float, vec2 ....)
  class ShaderUniformDeclaration
  {
  public:
    /// Virtual Default destcutor for Shader Uniform Declaration
    virtual ~ShaderUniformDeclaration() = default;
  };
  
  /// This class stores the data of structure in shader code
  class ShaderStruct
  {
  public:
    /// This constructor creates the structure instance for a shader
    /// - Parameter name: name of structure
    ShaderStruct(std::string_view name);
    /// This destructor destroys the structure data
    ~ShaderStruct();
    
  private:
    std::string m_name {};
    uint32_t m_size {0};
    uint32_t m_offset {0};
    std::vector<ShaderUniformDeclaration*> m_fields;
  };
} // namespace IKan
