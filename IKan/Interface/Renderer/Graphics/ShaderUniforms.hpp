//
//  ShaderUniforms.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  /// This enum stores the type of shader
  enum class ShaderDomain
  {
    None = 0,
    Vertex = 1,
    Fragment = 2,
    Geometry = 3
  };
  
  /// This is the interface class to store shader uniform elements (like int, float, vec ....)
  class ShaderUniformDeclaration
  {
  public:
    /// Virtual Default destcutor for Shader Uniform Declaration
    virtual ~ShaderUniformDeclaration() = default;
    
    /// This function returns the name of field
    virtual const std::string& GetName() const = 0;
    /// This functionr returns the size of field
    virtual uint32_t GetSize() const = 0;
    /// This function returns the count of field
    virtual uint32_t GetCount() const = 0;
    /// This function returns the offset of field
    virtual uint32_t GetOffset() const = 0;
    
    /// This function returns the domain of shader
    virtual ShaderDomain GetDomain() const = 0;
    
    friend class Shader;
    friend class OpenGLShader;
    friend class ShaderStruct;
    
  protected:
    /// This function update the offset of the field
    virtual void SetOffset(uint32_t offset) = 0;
  };
} // namespace IKan
