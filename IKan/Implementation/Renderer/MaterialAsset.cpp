//
//  MaterialAsset.cpp
//  IKan
//
//  Created by Ashish . on 10/09/23.
//

#include "MaterialAsset.hpp"
#include "Asset/AssetImporter.hpp"

namespace IKan
{
  Ref<MaterialAsset> MaterialAsset::Create(const std::string &materialShaderPath, const std::string &name)
  {
    return CreateRef<MaterialAsset>(materialShaderPath, name);
  }
  Ref<MaterialAsset> MaterialAsset::Create(const Ref<Shader> &shader, const std::string &name)
  {
    return CreateRef<MaterialAsset>(shader, name);
  }
  Ref<MaterialAsset> MaterialAsset::Create(const Ref<Material> &material, const std::string &name)
  {
    return CreateRef<MaterialAsset>(material, name);
  }
  MaterialAsset::MaterialAsset(const std::string &materialShaderPath, const std::string &name)
  : m_materialShaderPath(materialShaderPath), m_material(Material::Create(materialShaderPath, name))
  {
    
  }
  MaterialAsset::MaterialAsset(const Ref<Shader> &shader, const std::string &name)
  : m_materialShaderPath(shader->GetFilePath()), m_material(Material::Create(shader, name))
  {
    
  }
  MaterialAsset::MaterialAsset(const Ref<Material> &material, const std::string &name)
  : m_materialShaderPath(material->GetShader()->GetFilePath()), m_material(material)
  {
    
  }
  
  Ref<Material>& MaterialAsset::GetMaterial()
  {
    return m_material;
  }

  MaterialTable::MaterialTable()
  {
    
  }
  
  MaterialTable::~MaterialTable()
  {

  }

  void MaterialTable::SetMaterial(uint32_t index, Ref<MaterialAsset> material)
  {
    m_materials[index] = material;
  }
  void MaterialTable::Serialize()
  {
    for (const auto& [matIdx, material] : m_materials)
    {
      AssetImporter::Serialize(material);
    }
  }
  const std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterialAssets() const
  {
    return m_materials;
  }

  std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterialAssets()
  {
    return m_materials;
  }

} // namespace IKan
