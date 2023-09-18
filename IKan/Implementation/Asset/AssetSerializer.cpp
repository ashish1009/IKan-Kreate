//
//  AssetSerializer.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetSerializer.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Scene/Scene.hpp"
#include "Asset/AssetManager.hpp"
#include "Renderer/Mesh.hpp"

namespace IKan
{
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
      out << YAML::Key << "u_Material_AlbedoColor" << YAML::Value << material->Get<glm::vec3>("u_Material_AlbedoColor");
      out << YAML::Key << "u_Material_Metalness" << YAML::Value << material->Get<float>("u_Material_Metalness");
      out << YAML::Key << "u_Material_Roughness" << YAML::Value << material->Get<float>("u_Material_Roughness");
      
      Ref<Image> albedoMap = material->TryGetImage("u_AlbedoTexture");
      if (albedoMap)
      {
        out << YAML::Key << "u_AlbedoTexture" << YAML::Value << albedoMap->handle;
      }
      else
      {
        out << YAML::Key << "u_AlbedoTexture" << YAML::Value << 0;
      }
      out << YAML::Key << "u_AlbedoTextureToggle" << YAML::Value << material->Get<float>("u_AlbedoTextureToggle");

      Ref<Image> normalMap = material->TryGetImage("u_NormalTexture");
      if (normalMap)
      {
        out << YAML::Key << "u_NormalTexture" << YAML::Value << normalMap->handle;
      }
      else
      {
        out << YAML::Key << "u_NormalTexture" << YAML::Value << 0;
      }
      out << YAML::Key << "u_NormalTextureToggle" << YAML::Value << material->Get<float>("u_NormalTextureToggle");

      Ref<Image> roughnessMap = material->TryGetImage("u_RoughnessTexture");
      if (roughnessMap)
      {
        out << YAML::Key << "u_RoughnessTexture" << YAML::Value << roughnessMap->handle;
      }
      else
      {
        out << YAML::Key << "u_RoughnessTexture" << YAML::Value << 0;
      }
      out << YAML::Key << "u_RoughnessTextureToggle" << YAML::Value << material->Get<float>("u_RoughnessTextureToggle");

      Ref<Image> metallicMap = material->TryGetImage("u_MetallicTexture");
      if (metallicMap)
      {
        out << YAML::Key << "u_MetallicTexture" << YAML::Value << metallicMap->handle;
      }
      else
      {
        out << YAML::Key << "u_MetallicTexture" << YAML::Value << 0;
      }
      out << YAML::Key << "u_MetallicTextureToggle" << YAML::Value << material->Get<float>("u_MetallicTextureToggle");

      out << YAML::EndMap;
    }
    out << YAML::EndMap;
    
    std::ofstream fout(AssetManager::GetFileSystemPath(metadata));
    fout << out.c_str();

  }
  bool MeshMaterialSerializer::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const
  {
    return false;
  }

} // namespace IKan
