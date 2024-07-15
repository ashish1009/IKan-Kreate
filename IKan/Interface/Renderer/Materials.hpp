//
//  Materials.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Graphics/ShaderUniform.hpp"

namespace IKan
{
  /// This class stores the material data
  class Material
  {
  public:
    /// This constructor creates the material with shader
    /// - Parameter shader: shader reference
    Material(const Ref<Shader>& shader);
    /// This destructor destroys the material instance
    virtual ~Material();
    
    /// This function binds the material and upload all the shader data. Binds all the loaded image to shader slot.
    /// To be called before rendering any scene
    void Bind();
    /// This function unbinds the material. To be called after rendering any scene
    void Unbind();
    
    /// This function returns the name of Material
    const Ref<Shader>& GetShader() const;
    
    /// This fucntion uploads the date to material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - value: value
    template <typename T> void Set(const std::string& name, const T& value)
    {
      ShaderUniformDeclaration* decl = FindUniformDeclaration(name);
      Buffer& buffer = GetUniformBufferTarget(decl);
      buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
    }
    
    /// This fucntion returns the data from material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    template <typename T> T& Get(const std::string& name)
    {
      ShaderUniformDeclaration* decl = FindUniformDeclaration(name);
      Buffer& buffer = GetUniformBufferTarget(decl);
      return buffer.Read<T>(decl->GetOffset());
    }
    
    /// This function creates instance of Material with Shader instance
    /// - Parameter shader: shader instance
    [[nodiscard]] static Ref<Material> Create(const Ref<Shader>& shader);
    /// This funciton creates instance of Material with sahder file path
    /// - Parameter shaderFilePath: shader file path
    [[nodiscard]] static Ref<Material> Create(const std::filesystem::path& shaderFilePath);
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Material);
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function finds the uniform from shader
    /// - Parameter name: unfiorm name
    ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
    /// This function finds the resource from shader
    /// - Parameter name: resource name
    ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
    /// This funcreion returns the buffer stored for shader
    /// - Parameter uniformDeclaration: uniform declaration instance
    Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
    
    /// This funtion allocate memory to store shader data in buffer
    void AllocateStorage();
    
    // Member Functions ----------------------------------------------------------------------------------------------
    Ref<Shader> m_shader;
    
    Buffer m_vsUniformStorageBuffer;
    Buffer m_fsUniformStorageBuffer;
    Buffer m_gsUniformStorageBuffer;
  };
} // namespace IKan
