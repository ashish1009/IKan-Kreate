//
//  MeshMaterial.hpp
//  IKan
//
//  Created by Ashish . on 09/09/23.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Graphics/ShaderUniform.hpp"

namespace IKan
{
  /// This class store the material of shader
  class Material
  {
  public:
    /// @param shader shader instance
    Material(const Ref<Shader>& shader, const std::string& name);
    /// Destructor
    virtual ~Material();
    
    /// This function binds the material and upload all the shader data. Binds all the loaded image to shader slot.
    /// To be called before rendering any scene
    void Bind();
    /// This function unbinds the material. To be called after rendering any scene
    void Unbind();
    
    /// This function update the material name
    /// - Parameter name: name
    void SetName(const std::string& name);
    
    /// This function returns the name of Material
    const Ref<Shader>& GetShader() const;
    /// This function returns the shader binded to Material
    const std::string& GetName() const;
    
    /// This fucntion uploads the date to material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - value: value
    template <typename T> void Set(const std::string& name, const T& value)
    {
      auto decl = FindUniformDeclaration(name);
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
    }
    
    /// This fucntion returns the data from material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    template <typename T> T& Get(const std::string& name)
    {
      auto decl = FindUniformDeclaration(name);
      auto& buffer = GetUniformBufferTarget(decl);
      return buffer.Read<T>(decl->GetOffset());
    }
    
    /// This function uploads the image to material
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - image: image data
    void Set(const std::string& name, const Ref<Image>& image);
    /// This function returns the image from material
    /// - Parameters:
    ///   - name: name of uniform store in shader
    Ref<Image> TryGetImage(const std::string& name);

    /// This function creates instance of Material with Shader instance
    /// - Parameter shader: shader instance
    [[nodiscard]] static Ref<Material> Create(const Ref<Shader>& shader, const std::string& name);
    /// This funciton creates instance of Material with sahder file path
    /// - Parameter shader_file_path: shader file path
    [[nodiscard]] static Ref<Material> Create(const std::string& shaderFilePath, const std::string& name);
    
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
    /// This function binds all the image to be stored in shader material
    void BindImages();
    /// This function unbinds all the image to be stored in shader material
    void UnbindImages();
    
    // Member Functions ----------------------------------------------------------------------------------------------
    Ref<Shader> m_shader;
    std::string m_name;
    
    Buffer m_vsUniformStorageBuffer;
    Buffer m_fsUniformStorageBuffer;
    Buffer m_gsUniformStorageBuffer;
    
    std::vector<Ref<Image>> m_images;
  };
} // namespace IKan

