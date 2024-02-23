//
//  Material.cpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#include "Material.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  [[nodiscard]] Ref<Material> Material::Create(const Ref<Shader>& shader, const std::string& name)
  {
    return CreateRef<Material>(shader, name);
  }
  
  [[nodiscard]] Ref<Material> Material::Create(const std::filesystem::path& shaderFilePath, const std::string& name)
  {
    auto shader = ShaderLibrary::GetShader(shaderFilePath);
    return CreateRef<Material>(shader, name);
  }
  
  Material::Material(const Ref<Shader>& shader, const std::string& name)
  : m_shader(shader), m_name(name)
  {
    IK_PROFILE();
    IK_LOG_DEBUG(LogModule::Material, "Creating Material ...");
    IK_LOG_DEBUG(LogModule::Material, "  Name                    | {0}", m_name);
    IK_LOG_DEBUG(LogModule::Material, "  Shader                  | {0}", m_shader->GetName());
    AllocateStorage();
  }
  
  Material::~Material()
  {
    IK_PROFILE();
    IK_LOG_DEBUG(LogModule::Material, "Desroying Material !!!");
    IK_LOG_DEBUG(LogModule::Material, "  Shader | {0}", m_shader->GetName());
  }
  
  void Material::AllocateStorage()
  {
    IK_PROFILE();
    if (m_shader->HasVSMaterialUniformBuffer())
    {
      const auto& vsBuffer = m_shader->GetVSMaterialUniformBuffer();
      m_vsUniformStorageBuffer.Allocate(vsBuffer.GetSize());
      m_vsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_DEBUG(LogModule::Material, "  Vertex Shader Buffer    | {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_shader->HasFSMaterialUniformBuffer())
    {
      const auto& psBuffer = m_shader->GetFSMaterialUniformBuffer();
      m_fsUniformStorageBuffer.Allocate(psBuffer.GetSize());
      m_fsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_DEBUG(LogModule::Material, "  Fragment Shader Buffer  | {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_shader->HasGSMaterialUniformBuffer())
    {
      const auto& gsBuffer = m_shader->GetGSMaterialUniformBuffer();
      m_gsUniformStorageBuffer.Allocate(gsBuffer.GetSize());
      m_gsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_DEBUG(LogModule::Material, "  Geomatery Shader Buffer | {0} Bytes", gsBuffer.GetSize());
    }
  }
  
  ShaderUniformDeclaration* Material::FindUniformDeclaration(const std::string& name)
  {
    IK_PERFORMANCE("ShaderMaterial::FindUniformDeclaration");
    if (m_vsUniformStorageBuffer)
    {
      auto& declarations = m_shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
      for (ShaderUniformDeclaration* uniform : declarations)
      {
        if (uniform->GetName() == name)
        {
          return uniform;
        }
      }
    }
    
    if (m_fsUniformStorageBuffer)
    {
      auto& declarations = m_shader->GetFSMaterialUniformBuffer().GetUniformDeclarations();
      for (ShaderUniformDeclaration* uniform : declarations)
      {
        if (uniform->GetName() == name)
        {
          return uniform;
        }
      }
    }
    
    if (m_gsUniformStorageBuffer)
    {
      auto& declarations = m_shader->GetGSMaterialUniformBuffer().GetUniformDeclarations();
      for (ShaderUniformDeclaration* uniform : declarations)
      {
        if (uniform->GetName() == name)
        {
          return uniform;
        }
      }
    }
    IK_ASSERT(false, "Could not find uniform !!!");
    return nullptr;
  }
  
  Buffer& Material::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
  {
    IK_PERFORMANCE("ShaderMaterial::GetUniformBufferTarget");
    switch (uniformDeclaration->GetDomain())
    {
      case ShaderDomain::Vertex:    return m_vsUniformStorageBuffer;
      case ShaderDomain::Fragment:  return m_fsUniformStorageBuffer;
      case ShaderDomain::Geometry:  return m_gsUniformStorageBuffer;
      case ShaderDomain::None:
      default: IK_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
    }
  }
  
  ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
  {
    IK_PERFORMANCE("ShaderMaterial::FindResourceDeclaration");
    auto& resources = m_shader->GetResources();
    for (ShaderResourceDeclaration* resource : resources)
    {
      if (resource->GetName() == name)
      {
        return resource;
      }
    }
    IK_ASSERT(false, "Could not find texure uniform");
    return nullptr;
  }
  
  void Material::Set(const std::string& name, const Ref<Texture>& image)
  {
    IK_PERFORMANCE("ShaderMaterial::Set");
    auto decl = FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (m_textures.size() <= slot)
    {
      m_textures.resize((size_t)slot + 1);
    }
    m_textures[slot] = image;
  }
  
  Ref<Texture> Material:: TryGetImage(const std::string& name)
  {
    IK_PERFORMANCE("ShaderMaterial::TryGetImage");
    auto decl = FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (slot < m_textures.size())
    {
      return m_textures.at(slot);
    }
    return nullptr;
  }
  
  void Material::Bind()
  {
    IK_PERFORMANCE("ShaderMaterial::Bind");
    m_shader->Bind();
    
    if (m_vsUniformStorageBuffer)
    {
      m_shader->SetVSMaterialUniformBuffer(m_vsUniformStorageBuffer);
    }
    
    if (m_fsUniformStorageBuffer)
    {
      m_shader->SetFSMaterialUniformBuffer(m_fsUniformStorageBuffer);
    }
    
    if (m_gsUniformStorageBuffer)
    {
      m_shader->SetGSMaterialUniformBuffer(m_gsUniformStorageBuffer);
    }
    
    BindImages();
  }
  
  void Material::Unbind()
  {
    IK_PERFORMANCE("ShaderMaterial::Unbind");
    m_shader->Unbind();
    UnbindImages();
  }
  
  void Material::BindImages()
  {
    IK_PERFORMANCE("ShaderMaterial::BindImages");
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Bind((uint32_t)i);
      }
    }
  }
  
  void Material::UnbindImages()
  {
    IK_PERFORMANCE("ShaderMaterial::UnbindImages");
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Unbind();
      }
    }
  }
  
  void Material::SetName(const std::string& name)
  {
    m_name = name;
  }
  
  const Ref<Shader>& Material::GetShader() const
  {
    return m_shader;
  }
  
  const std::string& Material::GetName() const
  {
    return m_name;
  }
} // namespace ikan
