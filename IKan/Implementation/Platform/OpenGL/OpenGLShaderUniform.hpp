//
//  OpenGLShaderUniform.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Graphics/ShaderUniform.hpp"

namespace IKan
{
  /// This is the Open GL Implementation class to store the shader Fundamental unoforms. Stores the Type and count of
  /// data (single data from or outside struct)
  class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration
  {
  public:
    enum class Type
    {
      None, Float32, Vec2, Vec3, Vec4, Mat3, Mat4, Int32, Bool, Struct
    };
    
    /// This constructor creates the open GL Shader uniform field with fundamental type
    /// - Parameters:
    ///   - domain: Domain of shader
    ///   - type: type of uniform
    ///   - name: name of uniform
    OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);
    /// This constructor creates the open GL Shader uniform field with structure type
    /// - Parameters:
    ///   - domain: domain of shader
    ///   - uniformStruct: structure pointer
    ///   - name: name of structure
    OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32_t count = 1);
    
    /// This is the default destructor of Shader uniform
    virtual ~OpenGLShaderUniformDeclaration();
    
    /// This function returns the name of field
    const std::string& GetName() const override;
    /// This function returns the size of field
    uint32_t GetSize() const override;
    /// This function returns the count of field
    uint32_t GetCount() const override;
    /// This function returns the offset of field
    uint32_t GetOffset() const override;
    /// This function returns the domain of shader
    ShaderDomain GetDomain() const override;
    /// This function returns the otype of field
    Type GetType() const;
    /// This function returns true if field is array else false
    bool IsArray() const;
    /// This function returns the absolute offset of field in shader.
    uint32_t GetAbsoluteOffset() const;
    /// This function returns the location of field in the shader
    int32_t GetLocation(uint32_t index = 0) const;
    /// This function returns the structure pointer if field is structure
    const ShaderStruct& GetShaderUniformStruct() const;
    
    /// This static funciton returns the size of uniform field
    /// - Parameter type: type of field
    static uint32_t SizeOfUniformType(Type type);
    /// This function return the type with string type
    /// - Parameter type: typ in strug
    static Type StringToType(const std::string& type);
    /// This funciton reutrns the type in string from Type
    /// - Parameter type: Internal Type of field
    static std::string TypeToString(Type type);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLShaderUniformDeclaration);
    
  protected:
    /// This funciton updates the offset of field
    /// - Parameter offset: new offset
    void SetOffset(uint32_t offset) override;
    
  private:
    std::string m_name;
    uint32_t m_size, m_count, m_offset;
    Type m_type;
    ShaderDomain m_domain;
    ShaderStruct* m_struct;
    std::vector<int32_t> m_locations;
    
    friend class OpenGLShader;
    friend class OpenGLShaderUniformBufferDeclaration;
  };
} // namespace IKan
