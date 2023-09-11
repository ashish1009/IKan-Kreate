//
//  Material.hpp
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
  class MaterialInstance;
  
  /// This class store the material of shader
  class Material
  {
  public:
    /// @param shader shader instance
    Material(const Ref<Shader>& shader);
    /// Destructor
    virtual ~Material();
    
    /// This function binds the material and upload all the shader data. Binds all the loaded image to shader slot.
    /// To be called before rendering any scene
    void Bind();
    /// This function unbinds the material. To be called after rendering any scene
    void Unbind();
    
    /// This function returns the shader binded to Material
    const Ref<Shader>& GetShader() const;
    
    /// This fucntion uploads the date to material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - value: value
    template <typename T> void Set(const std::string& name, const T& value)
    {
      auto decl = FindUniformDeclaration(name);
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
      
      OnMaterialValueUpdated(decl);
    }
    
    /// This function uploads the image to material
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - image: image data
    void Set(const std::string& name, const Ref<Image>& image);
    
    /// This function creates instance of Material with Shader instance
    /// - Parameter shader: shader instance
    [[nodiscard]] static Ref<Material> Create(const Ref<Shader>& shader);
    /// This funciton creates instance of Material with sahder file path
    /// - Parameter shader_file_path: shader file path
    [[nodiscard]] static Ref<Material> Create(const std::string& shaderFilePath);
    
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
    
    /// This function updates the material in shader
    /// - Parameter decl: shader uniform
    void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
    /// This funtion allocate memory to store shader data in buffer
    void AllocateStorage();
    /// This function binds all the image to be stored in shader material
    void BindImages();
    /// This function unbinds all the image to be stored in shader material
    void UnbindImages();
    
    // Member Functions ----------------------------------------------------------------------------------------------
    Ref<Shader> m_shader;
    std::unordered_set<MaterialInstance*> m_materialInstances;
    
    Buffer m_vsUniformStorageBuffer;
    Buffer m_fsUniformStorageBuffer;
    Buffer m_gsUniformStorageBuffer;
    
    std::vector<Ref<Image>> m_images;
    
    friend class MaterialInstance;
  };
  
  /// This class stores the material of submesh of shader
  class MaterialInstance
  {
  public:
    /// This constructor creates instance of material instance
    /// - Parameters:
    ///   - material: material reference
    ///   - name: name of material
    MaterialInstance(const Ref<Material>& material, const std::string& name = "");
    /// This is defautl destructo of material instance
    virtual ~MaterialInstance();
    
    /// This function binds the material and upload all the shader data. Bind all the loaded image to shader slot.
    /// To be called before rendering any scene
    void Bind();
    /// This function unbinds the material and upload all the shader data. Bind all the loaded image to shader slot.
    /// To be called before rendering any scene
    void Unbind();
    
    /// This ducntion returns the shader binded to Material
    const Ref<Shader>& GetShader() const;
    
    /// This function returns the name of material
    const std::string& GetName() const;
    /// This function updates the name of material
    /// - Parameter name: new name of material
    void SetName(const std::string& name);
    
    /// This function uploads the date to material of type T
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - value: value
    template <typename T> void Set(const std::string& name, const T& value)
    {
      auto decl = m_material->FindUniformDeclaration(name);
      auto& buffer = GetUniformBufferTarget(decl);
      buffer.Write((std::byte*)& value, decl->GetSize(), decl->GetOffset());
      
      m_overriddenValues.insert(name);
    }
    
    /// This function uploads the image to material
    /// - Parameters:
    ///   - name: name of uniform store in shader
    ///   - image: image data
    void Set(const std::string& name, const Ref<Image>& image);
    
    /// This function creates instance of material instance with material reference and name
    /// - Parameters:
    ///   - material: material reference
    ///   - name: name of material
    [[nodiscard]] static Ref<MaterialInstance> Create(const Ref<Material>& material, const std::string& name = "");
    
  private:
    // Member functions ----------------------------------------------------------------------------------------------
    /// This funtion allocate memory to store shader data in buffer
    void AllocateStorage();
    /// This funcreion returns the buffer stored for shader
    /// - Parameter uniformDeclaration: uniform declaration instance
    Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
    /// This function updates the material in shader
    /// - Parameter decl: shader uniform
    void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
    
    // Member variables ----------------------------------------------------------------------------------------------
    Ref<Material> m_material;
    std::string m_name;
    
    Buffer m_vsUniformStorageBuffer;
    Buffer m_fsUniformStorageBuffer;
    Buffer m_gsUniformStorageBuffer;
    
    std::vector<Ref<Image>> m_images;
    
    std::unordered_set<std::string> m_overriddenValues;
    
    friend class Material;
  };
} // namespace IKan

