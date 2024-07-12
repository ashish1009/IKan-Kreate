//
//  OpenGLShaderUniform.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/ShaderUniform.hpp"

namespace IKan
{
  // OpenGLShaderUniformDeclaration ----------------------------------------------------------------------------------
  /// This class implements the APIs to store the Open Gl shader fundamental uniforms.
  /// - Note :Stores the Type and count of data (single data from or outside structs).
  class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration
  {
  public:
    /// This enum stores the type shader data type.
    enum class Type : uint8_t
    {
      None, Float32, Vec2, Vec3, Vec4, Mat3, Mat4, Int32, Bool, Struct
    };

    /// This constructor creates the open GL Shader uniform field with fundamental type.
    /// - Parameters:
    ///   - domain: Domain of shader.
    ///   - type: type of uniform.
    ///   - name: name of uniform.
    ///   - count: count of shader data type.
    OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);

    /// This destructor destroyes the Shader uniform.
    virtual ~OpenGLShaderUniformDeclaration();
    
    /// This function returns the name of field.
    const std::string& GetName() const override;
    /// This function returns the size of field.
    uint32_t GetSize() const override;
    /// This function returns the count of field.
    uint32_t GetCount() const override;
    /// This function returns the offset of field.
    uint32_t GetOffset() const override;
    /// This function returns the domain of shader.
    ShaderDomain GetDomain() const override;
    /// This function returns the otype of field.
    Type GetType() const;
    /// This function returns true if field is array else false.
    bool IsArray() const;
    /// This function returns the absolute offset of field in shader.
    uint32_t GetAbsoluteOffset() const;
    /// This function returns the location of field in the shader.
    /// - Parameter index: index of shader uniform.
    int32_t GetLocation(uint32_t index = 0) const;
    /// This function returns the structure pointer if field is structure.
    const ShaderStruct& GetShaderUniformStruct() const;
    
    /// This static funciton returns the size of uniform field.
    /// - Parameter type: type of field.
    static uint32_t SizeOfUniformType(Type type);
    /// This function returns the type with string type.
    /// - Parameter type: typ in strug.
    static Type StringToType(const std::string& type);
    /// This funciton reutrns the type in string from Type.
    /// - Parameter type: Internal Type of field.
    static std::string TypeToString(Type type);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLShaderUniformDeclaration);
    
  protected:
    /// This funciton updates the offset of field
    /// - Parameter offset: new offset
    void SetOffset(uint32_t offset) override;
    
  private:
    ShaderDomain m_domain {ShaderDomain::None};
    Type m_type {Type::None};
    std::string m_name {};
    uint32_t m_size{0}, m_count{0}, m_offset{0};
    ShaderStruct* m_struct;
    std::vector<int32_t> m_locations;

    friend class OpenGLShader;
    friend class OpenGLShaderUniformBufferDeclaration;
  };
} // namespace IKan
