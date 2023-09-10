//
//  Material.cpp
//  IKan
//
//  Created by Ashish . on 09/09/23.
//

#include "Material.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  [[nodiscard]] Ref<Material> Material::Create(const Ref<Shader>& shader)
  {
    return CreateRef<Material>(shader);
  }
  
  [[nodiscard]] Ref<Material> Material::Create(const std::string& shaderFilePath)
  {
    auto shader = Shader::Create(shaderFilePath);
    return CreateRef<Material>(shader);
  }
  
  Material::Material(const Ref<Shader>& shader)
  : m_shader(shader)
  {
    IK_LOG_TRACE(LogModule::Material, "Creating Material ...");
    IK_LOG_TRACE(LogModule::Material, "  Shader           | {0}", m_shader->GetName());
    AllocateStorage();
  }
  
  Material::~Material()
  {
    IK_LOG_WARN(LogModule::Material, "Desroying Material !!!");
    IK_LOG_WARN(LogModule::Material, "  Shader | {0}", m_shader->GetName());
  }
  
  void Material::AllocateStorage()
  {
    if (m_shader->HasVSMaterialUniformBuffer())
    {
      const auto& vsBuffer = m_shader->GetVSMaterialUniformBuffer();
      m_vsUniformStorageBuffer.Allocate(vsBuffer.GetSize());
      m_vsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_TRACE(LogModule::Material, "  Vertex Buffer    | {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_shader->HasFSMaterialUniformBuffer())
    {
      const auto& psBuffer = m_shader->GetFSMaterialUniformBuffer();
      m_fsUniformStorageBuffer.Allocate(psBuffer.GetSize());
      m_fsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_TRACE(LogModule::Material, "  Fragment Buffer  | {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_shader->HasGSMaterialUniformBuffer())
    {
      const auto& gsBuffer = m_shader->GetGSMaterialUniformBuffer();
      m_gsUniformStorageBuffer.Allocate(gsBuffer.GetSize());
      m_gsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_TRACE(LogModule::Material, "  Geomatery Buffer | {0} Bytes", gsBuffer.GetSize());
    }
  }
  
  ShaderUniformDeclaration* Material::FindUniformDeclaration(const std::string& name)
  {
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
  
  void Material::OnMaterialValueUpdated(ShaderUniformDeclaration* decl) {
    for (auto mi : m_materialInstances)
    {
      mi->OnMaterialValueUpdated(decl);
    }
  }
  
  void Material::Set(const std::string& name, const Ref<Texture>& texture)
  {
    auto decl = FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (m_textures.size() <= slot)
    {
      m_textures.resize((size_t)slot + 1);
    }
    m_textures[slot] = texture;
  }
  
  void Material::Bind()
  {
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
    
    BindTextures();
  }
  
  void Material::Unbind()
  {
    m_shader->Unbind();
    UnbindTextures();
  }
  
  void Material::BindTextures()
  {
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Bind((uint32_t)i);
      }
    }
  }
  
  void Material::UnbindTextures()
  {
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Unbind();
      }
    }
  }
  
  const Ref<Shader>& Material::GetShader() const
  {
    return m_shader;
  }
  
  [[nodiscard]] Ref<MaterialInstance> MaterialInstance::Create(const Ref<Material>& material, const std::string& name)
  {
    return CreateRef<MaterialInstance>(material, name);
  }
  
  MaterialInstance::MaterialInstance(const Ref<Material>& material, const std::string& name) :
  m_material(material), m_name(name)
  {
    IK_LOG_TRACE(LogModule::Material, "Creating MaterialInstance ...");
    IK_LOG_TRACE(LogModule::Material, "  Name               | {0}", m_name);
    IK_LOG_TRACE(LogModule::Material, "  Shader             | {0}", m_material->m_shader->GetName());
    m_material->m_materialInstances.insert(this);
    AllocateStorage();
  }
  
  MaterialInstance::~MaterialInstance()
  {
    IK_LOG_WARN(LogModule::Material, "Destroying MaterialInstance ...");
    IK_LOG_WARN(LogModule::Material, "  Name   | {0}", m_name);
    IK_LOG_WARN(LogModule::Material, "  Shader | {0}", m_material->m_shader->GetName());
    
    m_material->m_materialInstances.erase(this);
  }
  
  void MaterialInstance::AllocateStorage()
  {
    if (m_material->m_shader->HasVSMaterialUniformBuffer())
    {
      const auto& vsBuffer = m_material->m_shader->GetVSMaterialUniformBuffer();
      m_vsUniformStorageBuffer.Allocate(vsBuffer.GetSize());
      memcpy(m_vsUniformStorageBuffer.data, m_material->m_vsUniformStorageBuffer.data, vsBuffer.GetSize());
      IK_LOG_TRACE(LogModule::Material, "  Vertex Buffer      | {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_material->m_shader->HasFSMaterialUniformBuffer())
    {
      const auto& psBuffer = m_material->m_shader->GetFSMaterialUniformBuffer();
      m_fsUniformStorageBuffer.Allocate(psBuffer.GetSize());
      memcpy(m_fsUniformStorageBuffer.data, m_material->m_fsUniformStorageBuffer.data, psBuffer.GetSize());
      IK_LOG_TRACE(LogModule::Material, "  Fragment Buffer    | {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_material->m_shader->HasGSMaterialUniformBuffer())
    {
      const auto& gsBuffer = m_material->m_shader->GetGSMaterialUniformBuffer();
      m_gsUniformStorageBuffer.Allocate(gsBuffer.GetSize());
      memcpy(m_gsUniformStorageBuffer.data, m_material->m_gsUniformStorageBuffer.data, gsBuffer.GetSize());
      IK_LOG_TRACE(LogModule::Material, "  Geomatry Buffer    | {0} Bytes", gsBuffer.GetSize());
    }
  }
  
  void MaterialInstance::OnMaterialValueUpdated(ShaderUniformDeclaration* decl)
  {
    if (m_overriddenValues.find(decl->GetName()) == m_overriddenValues.end())
    {
      auto& buffer = GetUniformBufferTarget(decl);
      auto& materialBuffer = m_material->GetUniformBufferTarget(decl);
      buffer.Write(materialBuffer.data + decl->GetOffset(), decl->GetSize(), decl->GetOffset());
    }
  }
  
  Buffer& MaterialInstance::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
  {
    switch (uniformDeclaration->GetDomain()) {
      case ShaderDomain::Vertex:    return m_vsUniformStorageBuffer;
      case ShaderDomain::Fragment:  return m_fsUniformStorageBuffer;
      case ShaderDomain::Geometry:  return m_gsUniformStorageBuffer;
      case ShaderDomain::None:
      default:
        IK_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
    }
  }
  
  void MaterialInstance::Set(const std::string& name, const Ref<Texture>& texture)
  {
    auto decl = m_material->FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (m_textures.size() <= slot)
    {
      m_textures.resize((size_t)slot + 1);
    }
    m_textures[slot] = texture;
  }
  
  void MaterialInstance::Bind()
  {
    m_material->m_shader->Bind();
    m_material->BindTextures();
    
    if (m_vsUniformStorageBuffer)
    {
      m_material->m_shader->SetVSMaterialUniformBuffer(m_vsUniformStorageBuffer);
    }
    
    if (m_fsUniformStorageBuffer)
    {
      m_material->m_shader->SetFSMaterialUniformBuffer(m_fsUniformStorageBuffer);
    }
    
    if (m_gsUniformStorageBuffer)
    {
      m_material->m_shader->SetGSMaterialUniformBuffer(m_gsUniformStorageBuffer);
    }
    
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Bind((uint32_t)i);
      }
    }
  }
  
  void MaterialInstance::Unbind()
  {
    m_material->m_shader->Unbind();
    m_material->UnbindTextures();
    
    for (size_t i = 0; i < m_textures.size(); i++)
    {
      auto& texture = m_textures[i];
      if (texture)
      {
        texture->Unbind();
      }
    }
  }
  
  const std::string& MaterialInstance::GetName() const
  {
    return m_name;
  }
  void MaterialInstance::SetName(const std::string& name)
  {
    m_name = name;
  }
  
} // namespace ikan
