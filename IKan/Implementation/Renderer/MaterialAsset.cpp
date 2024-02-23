//
//  MaterialAsset.cpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#include "MaterialAsset.hpp"

namespace IKan
{
  MaterialAsset::MaterialAsset()
  {
    IK_PROFILE();
    m_material = Material::Create(ShaderLibrary::GetShader(CoreAsset("Shaders/PBR_StaticShader.glsl")), "PBR_Material");
    IK_LOG_INFO(LogModule::MaterialAsset, "Creating {0} Asset", m_material->GetName());
  }
  
  MaterialAsset::MaterialAsset(Ref<Material> material)
  : m_material(material)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::MaterialAsset, "Creating {0} Asset", m_material->GetName());
  }
  
  MaterialAsset::~MaterialAsset()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::MaterialAsset, "Destroying {0} Asset", m_material->GetName());
  }
  
  Ref<Material> MaterialAsset::GetMaterial() const
  {
    return m_material;
  }
  
  MaterialTable::MaterialTable(uint32_t materialCount)
  : m_materialCount(materialCount)
  {
  }
  
  MaterialTable::MaterialTable(Ref<MaterialTable> other)
  : m_materialCount(other->m_materialCount)
  {
    const auto& meshMaterials = other->GetMaterials();
    for (auto[index, materialAsset] : meshMaterials)
    {
      SetMaterial(index, materialAsset);
    }
  }

  bool MaterialTable::HasMaterial(uint32_t materialIndex) const
  {
    return m_materials.find(materialIndex) != m_materials.end();
  }
  
  Ref<MaterialAsset> MaterialTable::GetMaterial(uint32_t materialIndex) const
  {
    IK_ASSERT(HasMaterial(materialIndex));
    return m_materials.at(materialIndex);
  }

  void MaterialTable::SetMaterial(uint32_t index, Ref<MaterialAsset> material)
  {
    m_materials[index] = material;
    if (index >= m_materialCount)
    {
      m_materialCount = index + 1;
    }
  }

  void MaterialTable::ClearMaterial(uint32_t index)
  {
    IK_ASSERT(HasMaterial(index));
    m_materials.erase(index);
    if (index >= m_materialCount)
    {
      m_materialCount = index + 1;
    }
  }
  
  std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterials() 
  {
    return m_materials;
  }
  const std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterials() const
  {
    return m_materials;
  }
  
  uint32_t MaterialTable::GetMaterialCount() const 
  {
    return m_materialCount;
  }
  void MaterialTable::SetMaterialCount(uint32_t materialCount)
  {
    m_materialCount = materialCount;
  }
  
  void MaterialTable::Clear()
  {
    m_materials.clear();
  }

} // namespace IKan
