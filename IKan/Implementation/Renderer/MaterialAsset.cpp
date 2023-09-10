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
  MaterialAsset::MaterialAsset(const std::string &materialShaderPath)
  : m_materialShaderPath(materialShaderPath), m_material(Material::Create(materialShaderPath))
  {
    
  }
} // namespace IKan
