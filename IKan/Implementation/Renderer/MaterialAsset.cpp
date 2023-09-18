//
//  MaterialAsset.cpp
//  IKan
//
//  Created by Ashish . on 10/09/23.
//

#include "MaterialAsset.hpp"
#include "Asset/AssetImporter.hpp"
#include "Asset/AssetManager.hpp"

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
    m_material->SetName(name);
  }
  
  Ref<Material>& MaterialAsset::GetMaterial()
  {
    return m_material;
  }

  MaterialTable::MaterialTable(Ref<Material> baseMaterial)
  : m_baseMaterial(baseMaterial)
  {
    Initialize();
  }
  
  MaterialTable::MaterialTable(Ref<Shader> shader)
  : m_baseMaterial(Material::Create(shader, shader->GetName()))
  {
    Initialize();
  }
  
  MaterialTable::MaterialTable(const std::string& shaderFileName)
  : m_baseMaterial(Material::Create(shaderFileName, Utils::String::GetFileNameFromPath(shaderFileName)))
  {    
    Initialize();
  }
  
  MaterialTable::~MaterialTable()
  {

  }
  
  void MaterialTable::Initialize()
  {
    m_baseMaterial->Set("u_Material_AlbedoColor", glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
    m_baseMaterial->Set("u_Material_Metalness", 0.5);
    m_baseMaterial->Set("u_Material_Roughness", 0.5);
  }
  
  AssetHandle MaterialTable::CreateNewMaterialAsset(const std::filesystem::path& materialBaseDir, const std::string& materialName)
  {
    std::string materialNameWithExtension = materialName + std::string(".ikmat");
    std::filesystem::path materialAssetFile = materialBaseDir / materialNameWithExtension;
    
    AssetHandle materialAssetHandle = AssetManager::CreateAsset<MaterialAsset>(materialAssetFile, m_baseMaterial, materialName);
    m_materialAssetess[m_materialCount++] = AssetManager::GetAsset<MaterialAsset>(materialAssetHandle);
    
    return materialAssetHandle;
  }
  
  void MaterialTable::AddMaterialAsset(const AssetHandle &handle)
  {
    m_materialAssetess[m_materialCount++] = AssetManager::GetAsset<MaterialAsset>(handle);
  }

  void MaterialTable::SetMaterialAsset(uint32_t index, Ref<MaterialAsset> materialAsset)
  {
    m_materialAssetess[index] = materialAsset;
  }
  void MaterialTable::Serialize()
  {
    for (const auto& [matIdx, materialAsset] : m_materialAssetess)
    {
      AssetImporter::Serialize(materialAsset);
    }
  }
  const std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterialAssets() const
  {
    return m_materialAssetess;
  }

  std::map<uint32_t, Ref<MaterialAsset>>& MaterialTable::GetMaterialAssets()
  {
    return m_materialAssetess;
  }
  
  uint32_t MaterialTable::GetSize() const
  {
    IK_ASSERT(m_materialCount == m_materialAssetess.size());
    return static_cast<uint32_t>(m_materialAssetess.size());
  }

} // namespace IKan
