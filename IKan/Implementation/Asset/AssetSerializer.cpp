//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetSerializer.hpp"
#include "Utils/YAMLSerializerHelper.h"
#include "Renderer/MaterialAsset.hpp"
#include "Renderer/MeshMaterial.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/Scene.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Mesh.hpp"

namespace IKan
{
#define SERIALIZE_MATERIAL_TEX(Name) \
{ \
  Ref<Image> map = material->TryGetImage(#Name); \
  IK_SERIALIZE_PROPERTY_ASSET(Name, map, out); \
\
  std::string toggleMapName = #Name; \
  toggleMapName += "Toggle"; \
  out << YAML::Key << toggleMapName << YAML::Value << material->Get<float>(toggleMapName); \
}
  
#define DESERIALIZE_MATERIAL_TEX(Name) \
{ \
  std::string toggleMapName = #Name; \
  toggleMapName += "Toggle"; \
  materialRef->Set(toggleMapName, material[toggleMapName].as<float>()); \
  if (material[#Name].as<AssetHandle>() > 0) \
  { \
    materialRef->Set(#Name, AssetManager::GetAsset<Image>(material[#Name].as<AssetHandle>())); \
  } \
}

  bool ImageSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Image::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  
  bool SceneAssetSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Scene::Create();
    asset->handle = metadata.handle;
    return true;
  }
  
  bool FontSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Font::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  
  bool MeshSourceSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = MeshSource::Create(AssetManager::GetFileSystemPathString(metadata));
    asset->handle = metadata.handle;
    return true;
  }

  void MeshMaterialSerializer::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const
  {
    const auto& material = std::dynamic_pointer_cast<MaterialAsset>(asset)->GetMaterial();
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Material" << YAML::Value;
    {
      out << YAML::BeginMap;
      out << YAML::Key << "Handle" << YAML::Value << asset->handle;
      out << YAML::Key << "Name" << YAML::Value << material->GetName();
      out << YAML::Key << "ShaderPath" << YAML::Value << Utils::FileSystem::IKanAbsolute(material->GetShader()->GetFilePath());

      out << YAML::Key << "u_Material_AlbedoColor" << YAML::Value << material->Get<glm::vec3>("u_Material_AlbedoColor");
      out << YAML::Key << "u_Material_Metalness" << YAML::Value << material->Get<float>("u_Material_Metalness");
      out << YAML::Key << "u_Material_Roughness" << YAML::Value << material->Get<float>("u_Material_Roughness");
      out << YAML::Key << "u_TilingFactor" << YAML::Value << material->Get<float>("u_TilingFactor");

      SERIALIZE_MATERIAL_TEX(u_AlbedoTexture);
      SERIALIZE_MATERIAL_TEX(u_NormalTexture);
      SERIALIZE_MATERIAL_TEX(u_RoughnessTexture);
      SERIALIZE_MATERIAL_TEX(u_MetallicTexture);

      out << YAML::EndMap;
    }
    out << YAML::EndMap;
    
    std::ofstream fout(AssetManager::GetFileSystemPath(metadata));
    fout << out.c_str();
  }
  
  bool MeshMaterialSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
  {
    std::ifstream stream(AssetManager::GetFileSystemPath(metadata));
    IK_ASSERT(stream);
    
    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node data = YAML::Load(strStream.str());
    auto material = data["Material"];
    if (!material)
    {
      return false;
    }

    std::string name = material["Name"].as<std::string>();
    std::string shaderPath = material["ShaderPath"].as<std::string>();
    
    asset = MaterialAsset::Create(shaderPath, name);
    asset->handle = material["Handle"].as<AssetHandle>();

    Ref<MaterialAsset> materialAsset = std::dynamic_pointer_cast<MaterialAsset>(asset);
    Ref<Material> materialRef = materialAsset->GetMaterial();
    
    materialRef->Set("u_Material_AlbedoColor", material["u_Material_AlbedoColor"].as<glm::vec3>());
    materialRef->Set("u_Material_Metalness", material["u_Material_Metalness"].as<float>());
    materialRef->Set("u_Material_Roughness", material["u_Material_Roughness"].as<float>());
    
    DESERIALIZE_MATERIAL_TEX(u_AlbedoTexture);
    DESERIALIZE_MATERIAL_TEX(u_NormalTexture);
    DESERIALIZE_MATERIAL_TEX(u_RoughnessTexture);
    DESERIALIZE_MATERIAL_TEX(u_MetallicTexture);

    return true;
  }

} // namespace IKan
