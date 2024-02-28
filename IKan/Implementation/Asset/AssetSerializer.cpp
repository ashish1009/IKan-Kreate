//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "AssetSerializer.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/Scene.hpp"

namespace IKan
{
#define SerializeTextureMap(title, material) \
{\
Ref<Image> map = material->Get##title##Map(); \
AssetHandle mapHandle = map ? map->handle : (AssetHandle)0; \
std::string uniform = std::string(#title)+"Texture"; \
out << YAML::Key << uniform << YAML::Value << mapHandle; \
}
  
#define IK_DESERIALIZE_PROPERTY(propName, destination, node, defaultValue) destination = node[#propName] ? node[#propName].as<decltype(defaultValue)>() : defaultValue

  bool ImageSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = std::dynamic_pointer_cast<Image>(TextureFactory::Create(AssetManager::GetFileSystemPath(metadata)));
    asset->handle = metadata.handle;
    return true;
  }
  bool MeshSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Mesh::Create(AssetManager::GetFileSystemPath(metadata));
    asset->handle = metadata.handle;
    return true;
  }
  bool SceneAssetSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    asset = Scene::Create();
    asset->handle = metadata.handle;
    return true;
  }
  void MaterialSerializer::Serialize(const AssetMetadata &metadata, const Ref<Asset> &asset) const
  {
    Ref<MaterialAsset> material = std::dynamic_pointer_cast<MaterialAsset>(asset);

    YAML::Emitter out;
    out << YAML::BeginMap; // Material
    out << YAML::Key << "Material" << YAML::Value;
    {
      out << YAML::BeginMap;

      out << YAML::Key << "AlbedoColor" << YAML::Value <<  material->GetAlbedoColor();
      out << YAML::Key << "Metallic" << YAML::Value <<  material->GetMetalness();
      out << YAML::Key << "Roughness" << YAML::Value <<  material->GetRoughness();
      out << YAML::Key << "DepthScale" << YAML::Value <<  material->GetDepthScale();
      out << YAML::Key << "TilingFactor" << YAML::Value <<  material->GetTilingFactor();

      out << YAML::Key << "AlbedoTextureToggle" << YAML::Value <<  material->GetAlbedoMapToggle();
      out << YAML::Key << "NormalTextureToggle" << YAML::Value <<  material->GetNormalMapToggle();
      out << YAML::Key << "RoughnessTextureToggle" << YAML::Value <<  material->GetRoughnessMapToggle();
      out << YAML::Key << "MetallicTextureToggle" << YAML::Value <<  material->GetMetallicMapToggle();
      out << YAML::Key << "DepthTextureToggle" << YAML::Value <<  material->GetDepthMapToggle();
      out << YAML::Key << "AoTextureToggle" << YAML::Value <<  material->GetAoMapToggle();

      SerializeTextureMap(Albedo, material);
      SerializeTextureMap(Normal, material);
      SerializeTextureMap(Metallic, material);
      SerializeTextureMap(Roughness, material);
      SerializeTextureMap(Depth, material);
      SerializeTextureMap(Ao, material);
      out << YAML::EndMap;
    }
    out << YAML::EndMap; // Material

    std::ofstream fout(AssetManager::GetFileSystemPath(metadata));
    fout << out.c_str();
  }
  bool MaterialSerializer::TryLoadData(const AssetMetadata &metadata, Ref<Asset> &asset) const
  {
    std::ifstream stream(AssetManager::GetFileSystemPath(metadata));
    if (!stream.is_open())
    {
      return false;
    }
    
    std::stringstream strStream;
    strStream << stream.rdbuf();
    
    YAML::Node root = YAML::Load(strStream.str());
    YAML::Node materialNode = root["Material"];
    
    Ref<MaterialAsset> material = MaterialAsset::Create();
    
    IK_DESERIALIZE_PROPERTY(AlbedoColor, material->GetAlbedoColor(), materialNode, glm::vec3(0.8f));
    IK_DESERIALIZE_PROPERTY(Metalness, material->GetMetalness(), materialNode, 0.5f);
    IK_DESERIALIZE_PROPERTY(Roughness, material->GetRoughness(), materialNode, 0.5f);
    IK_DESERIALIZE_PROPERTY(DepthScale, material->GetDepthScale(), materialNode, 0.001f);
    IK_DESERIALIZE_PROPERTY(TilingFactor, material->GetTilingFactor(), materialNode, 1.0f);

    material->SetAlbedoMapToggle(materialNode["AlbedoTextureToggle"] ? materialNode["AlbedoTextureToggle"].as<float>() : 0.0f);
    material->SetNormalMapToggle(materialNode["NormalTextureToggle"] ? materialNode["NormalTextureToggle"].as<float>() : 0.0f);
    material->SetRoughnessMapToggle(materialNode["RoughnessTextureToggle"] ? materialNode["RoughnessTextureToggle"].as<float>() : 0.0f);
    material->SetMetallicMapToggle(materialNode["MetallicTextureToggle"] ? materialNode["MetallicTextureToggle"].as<float>() : 0.0f);
    material->SetDepthMapToggle(materialNode["DepthTextureToggle"] ? materialNode["DepthTextureToggle"].as<float>() : 0.0f);
    material->SetAoMapToggle(materialNode["AoTextureToggle"] ? materialNode["AoTextureToggle"].as<float>() : 0.0f);

    AssetHandle albedoMap, normalMap, metalnessMap, roughnessMap, depthMap, aoMap;
    IK_DESERIALIZE_PROPERTY(AlbedoMap, albedoMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(NormalMap, normalMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(MetalnessMap, metalnessMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(RoughnessMap, roughnessMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(DepthMap, depthMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(AoMap, aoMap, materialNode, (AssetHandle)0);

    if (albedoMap)
    {
      if (AssetManager::IsAssetHandleValid(albedoMap))
      {
        material->SetAlbedoMap(AssetManager::GetAsset<Image>(albedoMap));
      }
    }
    if (normalMap)
    {
      if (AssetManager::IsAssetHandleValid(normalMap))
      {
        material->SetNormalMap(AssetManager::GetAsset<Image>(normalMap));
      }
    }
    if (metalnessMap)
    {
      if (AssetManager::IsAssetHandleValid(metalnessMap))
      {
        material->SetMetallicMap(AssetManager::GetAsset<Image>(metalnessMap));
      }
    }
    if (roughnessMap)
    {
      if (AssetManager::IsAssetHandleValid(roughnessMap))
      {
        material->SetRoughnessMap(AssetManager::GetAsset<Image>(roughnessMap));
      }
    }
    if (depthMap)
    {
      if (AssetManager::IsAssetHandleValid(depthMap))
      {
        material->SetDepthMap(AssetManager::GetAsset<Image>(depthMap));
      }
    }
    if (aoMap)
    {
      if (AssetManager::IsAssetHandleValid(aoMap))
      {
        material->SetAoMap(AssetManager::GetAsset<Image>(aoMap));
      }
    }
    
    asset = material;
    asset->handle = metadata.handle;
    
    return true;
  }
} // namespace IKan
