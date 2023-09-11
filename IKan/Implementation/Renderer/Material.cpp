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
  [[nodiscard]] Ref<Material> Material::Create(const Ref<Shader>& shader, const std::string& name)
  {
    return CreateRef<Material>(shader, name);
  }
  
  [[nodiscard]] Ref<Material> Material::Create(const std::string& shaderFilePath, const std::string& name)
  {
    auto shader = Shader::Create(shaderFilePath);
    return CreateRef<Material>(shader, name);
  }
  
  Material::Material(const Ref<Shader>& shader, const std::string& name)
  : m_shader(shader), m_name(name)
  {
    IK_LOG_TRACE(LogModule::Material, "Creating Material ...");
    IK_LOG_TRACE(LogModule::Material, "  Name                    | {0}", m_name);
    IK_LOG_TRACE(LogModule::Material, "  Shader                  | {0}", m_shader->GetName());
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
      IK_LOG_TRACE(LogModule::Material, "  Vertex Shader Buffer    | {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_shader->HasFSMaterialUniformBuffer())
    {
      const auto& psBuffer = m_shader->GetFSMaterialUniformBuffer();
      m_fsUniformStorageBuffer.Allocate(psBuffer.GetSize());
      m_fsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_TRACE(LogModule::Material, "  Fragment Shader Buffer  | {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_shader->HasGSMaterialUniformBuffer())
    {
      const auto& gsBuffer = m_shader->GetGSMaterialUniformBuffer();
      m_gsUniformStorageBuffer.Allocate(gsBuffer.GetSize());
      m_gsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_TRACE(LogModule::Material, "  Geomatery Shader Buffer | {0} Bytes", gsBuffer.GetSize());
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
  
  void Material::Set(const std::string& name, const Ref<Image>& image)
  {
    auto decl = FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (m_images.size() <= slot)
    {
      m_images.resize((size_t)slot + 1);
    }
    m_images[slot] = image;
  }
  
  Ref<Image> Material:: TryGetImage(const std::string& name)
  {
    auto decl = FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (slot < m_images.size())
    {
      return m_images.at(slot);
    }
    return nullptr;
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
    
    BindImages();
  }
  
  void Material::Unbind()
  {
    m_shader->Unbind();
    UnbindImages();
  }
  
  void Material::BindImages()
  {
    for (size_t i = 0; i < m_images.size(); i++)
    {
      auto& image = m_images[i];
      if (image)
      {
        image->Bind((uint32_t)i);
      }
    }
  }
  
  void Material::UnbindImages()
  {
    for (size_t i = 0; i < m_images.size(); i++)
    {
      auto& image = m_images[i];
      if (image)
      {
        image->Unbind();
      }
    }
  }
  
  const Ref<Shader>& Material::GetShader() const
  {
    return m_shader;
  }
  
  const std::string& Material::GetName() const
  {
    return m_name;
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
  
  void MaterialInstance::Set(const std::string& name, const Ref<Image>& image)
  {
    auto decl = m_material->FindResourceDeclaration(name);
    
    uint32_t slot = decl->GetRegister();
    if (m_images.size() <= slot)
    {
      m_images.resize((size_t)slot + 1);
    }
    m_images[slot] = image;
  }
  
  void MaterialInstance::Bind()
  {
    m_material->m_shader->Bind();
    m_material->BindImages();
    
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
    
    for (size_t i = 0; i < m_images.size(); i++)
    {
      auto& image = m_images[i];
      if (image)
      {
        image->Bind((uint32_t)i);
      }
    }
  }
  
  void MaterialInstance::Unbind()
  {
    m_material->m_shader->Unbind();
    m_material->UnbindImages();
    
    for (size_t i = 0; i < m_images.size(); i++)
    {
      auto& image = m_images[i];
      if (image)
      {
        image->Unbind();
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
