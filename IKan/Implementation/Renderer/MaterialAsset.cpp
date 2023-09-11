//
//  MaterialAsset.cpp
//  IKan
//
//  Created by Ashish . on 10/09/23.
//

#include "MaterialAsset.hpp"

namespace IKan
{
  Ref<MaterialAsset> MaterialAsset::Create(const std::string &materialShaderPath)
  {
    return CreateRef<MaterialAsset>(materialShaderPath);
  }
  Ref<MaterialAsset> MaterialAsset::Create(const Ref<Shader> &shader)
  {
    return CreateRef<MaterialAsset>(shader);
  }
  Ref<MaterialAsset> MaterialAsset::Create(const Ref<Material> &material)
  {
    return CreateRef<MaterialAsset>(material);
  }
  MaterialAsset::MaterialAsset(const std::string &materialShaderPath)
  : m_materialShaderPath(materialShaderPath), m_material(Material::Create(materialShaderPath))
  {
    
  }
  MaterialAsset::MaterialAsset(const Ref<Shader> &shader)
  : m_materialShaderPath(shader->GetFilePath()), m_material(Material::Create(shader))
  {
    
  }
  MaterialAsset::MaterialAsset(const Ref<Material> &material)
  : m_materialShaderPath(material->GetShader()->GetFilePath()), m_material(material)
  {
    
  }

  MaterialTable::MaterialTable(uint32_t materialCount)
  : m_materialCount(materialCount)
  {
    
  }
  
  MaterialTable::~MaterialTable()
  {

  }

} // namespace IKan
