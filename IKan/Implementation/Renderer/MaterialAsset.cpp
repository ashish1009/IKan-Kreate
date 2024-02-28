//
//  MaterialAsset.cpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#include "MaterialAsset.hpp"
#include "Asset/AssetManager.hpp"
#include "Project/Project.hpp"

namespace IKan
{
#define DefineTextureMapAPI(title) \
float& MaterialAsset::Get##title##MapToggle() { std::string uniform = "u_"+std::string(#title)+"TextureToggle"; return m_material->Get<float>(uniform); } \
void MaterialAsset::Set##title##MapToggle(float value) { std::string uniform = "u_"+std::string(#title)+"TextureToggle"; m_material->Set<float>(uniform, value); }\
Ref<Image> MaterialAsset::Get##title##Map() { std::string uniform = "u_"+std::string(#title)+"Texture"; return m_material->TryGetImage(uniform); } \
void MaterialAsset::Set##title##Map(Ref<Image> tex) { std::string uniform = "u_"+std::string(#title)+"Texture"; m_material->Set(uniform, tex); } \

  Ref<MaterialAsset> MaterialAsset::Create()
  {
    return CreateRef<MaterialAsset>();
  }
  
  MaterialAsset::MaterialAsset()
  {
    IK_PROFILE();
    m_material = Material::Create(ShaderLibrary::GetShader(CoreAsset("Shaders/PBR_StaticShader.glsl")), "PBR_Material");
    IK_LOG_INFO(LogModule::MaterialAsset, "Creating {0} Asset", m_material->GetName());
    
    SetAlbedoColor({0.8f, 0.8f, 0.8f});
    SetMetalness(0.5);
    SetRoughness(0.5);
    SetDepthScale(0.001);
    SetTilingFactor(1.0f);
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
  
  glm::vec3& MaterialAsset::GetAlbedoColor()
  {
    return m_material->Get<MaterialProperty>("u_Material").color;
  }
  
  void MaterialAsset::SetAlbedoColor(const glm::vec3& color)
  {
    auto& mat = m_material->Get<MaterialProperty>("u_Material");
    mat.color = color;
    return m_material->Set<MaterialProperty>("u_Material", mat);
  }
  
  float& MaterialAsset::GetMetalness()
  {
    return m_material->Get<MaterialProperty>("u_Material").metallic;
  }
  
  void MaterialAsset::SetMetalness(float value)
  {
    auto& mat = m_material->Get<MaterialProperty>("u_Material");
    mat.metallic = value;
    return m_material->Set<MaterialProperty>("u_Material", mat);
  }
  
  float& MaterialAsset::GetRoughness()
  {
    return m_material->Get<MaterialProperty>("u_Material").roughness;
  }
  
  void MaterialAsset::SetRoughness(float value)
  {
    auto& mat = m_material->Get<MaterialProperty>("u_Material");
    mat.roughness = value;
    return m_material->Set<MaterialProperty>("u_Material", mat);
  }
  
  float& MaterialAsset::GetDepthScale()
  {
    return m_material->Get<MaterialProperty>("u_Material").depthScale;
  }
  
  void MaterialAsset::SetDepthScale(float value)
  {
    auto& mat = m_material->Get<MaterialProperty>("u_Material");
    mat.depthScale = value;
    return m_material->Set<MaterialProperty>("u_Material", mat);
  }
  
  float& MaterialAsset::GetTilingFactor()
  {
    return m_material->Get<MaterialProperty>("u_Material").tilingFactor;
  }
  
  void MaterialAsset::SetTilingFactor(float value)
  {
    auto& mat = m_material->Get<MaterialProperty>("u_Material");
    mat.tilingFactor = value;
    return m_material->Set<MaterialProperty>("u_Material", mat);
  }
  
  DefineTextureMapAPI(Albedo)
  DefineTextureMapAPI(Normal)
  DefineTextureMapAPI(Metallic)
  DefineTextureMapAPI(Roughness)
  DefineTextureMapAPI(Depth)
  DefineTextureMapAPI(Ao)

  Ref<Material> MaterialAsset::GetMaterial()
  {
    return m_material;
  }
  
  // Material Table --------------------------------------------------------------------------------------------------
  
  MaterialTable::MaterialTable()
  : m_materialCount(1)
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
