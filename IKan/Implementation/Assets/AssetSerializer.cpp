//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "AssetSerializer.hpp"

#include "Assets/AssetManager.hpp"

#include "Renderer/Mesh.hpp"
#include "Renderer/Graphics/Texture.hpp"

#include "Scene/Scene.hpp"
#include "Scene/Component.hpp"
#include "Scene/EntitySerializer.hpp"

#include "Project/Project.hpp"

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
    asset = Scene::Create(Project::GetActive()->GetConfig().sceneType);
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
      out << YAML::Key << "Emission" << YAML::Value <<  material->GetEmission();
      out << YAML::Key << "Roughness" << YAML::Value <<  material->GetRoughness();
      out << YAML::Key << "DepthScale" << YAML::Value <<  material->GetDepthScale();
      
      out << YAML::Key << "AlbedoTextureToggle" << YAML::Value <<  material->GetAlbedoMapToggle();
      out << YAML::Key << "NormalTextureToggle" << YAML::Value <<  material->GetNormalMapToggle();
      out << YAML::Key << "RoughnessTextureToggle" << YAML::Value <<  material->GetRoughnessMapToggle();
      out << YAML::Key << "MetallicTextureToggle" << YAML::Value <<  material->GetMetallicMapToggle();
      out << YAML::Key << "DepthTextureToggle" << YAML::Value <<  material->GetDepthMapToggle();
      
      SerializeTextureMap(Albedo, material);
      SerializeTextureMap(Normal, material);
      SerializeTextureMap(Metallic, material);
      SerializeTextureMap(Roughness, material);
      SerializeTextureMap(Depth, material);
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
    IK_DESERIALIZE_PROPERTY(Metallic, material->GetMetalness(), materialNode, 0.5f);
    IK_DESERIALIZE_PROPERTY(Emission, material->GetEmission(), materialNode, 0.0f);
    IK_DESERIALIZE_PROPERTY(Roughness, material->GetRoughness(), materialNode, 0.5f);
    IK_DESERIALIZE_PROPERTY(DepthScale, material->GetDepthScale(), materialNode, 0.001f);
    
    material->SetAlbedoMapToggle(materialNode["AlbedoTextureToggle"] ? materialNode["AlbedoTextureToggle"].as<float>() : 0.0f);
    material->SetNormalMapToggle(materialNode["NormalTextureToggle"] ? materialNode["NormalTextureToggle"].as<float>() : 0.0f);
    material->SetRoughnessMapToggle(materialNode["RoughnessTextureToggle"] ? materialNode["RoughnessTextureToggle"].as<float>() : 0.0f);
    material->SetMetallicMapToggle(materialNode["MetallicTextureToggle"] ? materialNode["MetallicTextureToggle"].as<float>() : 0.0f);
    material->SetDepthMapToggle(materialNode["DepthTextureToggle"] ? materialNode["DepthTextureToggle"].as<float>() : 0.0f);
    
    AssetHandle albedoMap, normalMap, metalnessMap, roughnessMap, depthMap, aoMap;
    IK_DESERIALIZE_PROPERTY(AlbedoTexture, albedoMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(NormalTexture, normalMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(MetallicTexture, metalnessMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(RoughnessTexture, roughnessMap, materialNode, (AssetHandle)0);
    IK_DESERIALIZE_PROPERTY(DepthTexture, depthMap, materialNode, (AssetHandle)0);
    
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
    
    asset = material;
    asset->handle = metadata.handle;
    return true;
  }
} // namespace IKan
