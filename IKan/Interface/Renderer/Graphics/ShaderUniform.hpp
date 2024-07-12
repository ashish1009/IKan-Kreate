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
  
  /// This class stores the data of structure in shader code
  class ShaderStruct
  {
  public:
    /// This constructor creates the structure instance for a shader
    /// - Parameter name: name of structure
    ShaderStruct(std::string_view name);
    /// This destructor destroys the structure data
    ~ShaderStruct();
    
    /// This function updates the offset value of structure
    /// - Parameter offset: new offset
    void SetOffset(uint32_t offset);
    
    /// This function returns the size of structure in bytes
    uint32_t GetSize() const;
    /// This function returns the offset of structure
    uint32_t GetOffset() const;
    /// This function returns the name of structure
    const std::string& GetName() const;
    /// This function returns the all the fields of structure
    const std::vector<ShaderUniformDeclaration*>& GetFields() const;
    
    /// This function adds a new field in structure
    void AddField(ShaderUniformDeclaration* field);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ShaderStruct)
    
  private:
    std::string m_name {};
    uint32_t m_size {0};
    uint32_t m_offset {0};
    std::vector<ShaderUniformDeclaration*> m_fields;
  };
} // namespace IKan
