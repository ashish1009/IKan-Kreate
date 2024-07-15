//
//  Materials.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "Materials.hpp"

#include "Renderer/Renderer.hpp"

namespace IKan
{
  [[nodiscard]] Ref<Material> Material::Create(const Ref<Shader>& shader)
  {
    return CreateRef<Material>(shader);
  }
  
  [[nodiscard]] Ref<Material> Material::Create(const std::filesystem::path& shaderFilePath)
  {
    return CreateRef<Material>(ShaderLibrary::Get(shaderFilePath));
  }
  
  Material::Material(const Ref<Shader>& shader)
  : m_shader(shader)
  {
    IK_PROFILE();
    IK_LOG_DEBUG(LogModule::Material, "Creating Material ...");
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
      const ShaderUniformBufferDeclaration& vsBuffer = m_shader->GetVSMaterialUniformBuffer();
      m_vsUniformStorageBuffer.Allocate(vsBuffer.GetSize());
      m_vsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_DEBUG(LogModule::Material, "  Vertex Shader Buffer    | {0} Bytes", vsBuffer.GetSize());
    }
    
    if (m_shader->HasFSMaterialUniformBuffer())
    {
      const ShaderUniformBufferDeclaration& psBuffer = m_shader->GetFSMaterialUniformBuffer();
      m_fsUniformStorageBuffer.Allocate(psBuffer.GetSize());
      m_fsUniformStorageBuffer.ZeroInitialize();
      IK_LOG_DEBUG(LogModule::Material, "  Fragment Shader Buffer  | {0} Bytes", psBuffer.GetSize());
    }
    
    if (m_shader->HasGSMaterialUniformBuffer())
    {
      const ShaderUniformBufferDeclaration& gsBuffer = m_shader->GetGSMaterialUniformBuffer();
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
      const std::vector<ShaderUniformDeclaration*>& declarations = m_shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
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
      const std::vector<ShaderUniformDeclaration*>& declarations = m_shader->GetFSMaterialUniformBuffer().GetUniformDeclarations();
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
      const std::vector<ShaderUniformDeclaration*>& declarations = m_shader->GetGSMaterialUniformBuffer().GetUniformDeclarations();
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
    assert(false);
  }
  
  ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
  {
    IK_PERFORMANCE("ShaderMaterial::FindResourceDeclaration");
    const std::vector<ShaderResourceDeclaration*>& resources = m_shader->GetResources();
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
  }
  
  void Material::Unbind()
  {
    IK_PERFORMANCE("ShaderMaterial::Unbind");
    m_shader->Unbind();
  }
    
  const Ref<Shader>& Material::GetShader() const
  {
    return m_shader;
  }
} // namespace ikan
