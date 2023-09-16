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
//      out << YAML::Key << "Name" << YAML::Value << m_project->m_config.name;
//      out << YAML::Key << "AssetDirectory" << YAML::Value << m_project->m_config.assetDirectory;
//      out << YAML::Key << "MeshDirectory" << YAML::Value << m_project->m_config.meshPath;
//      out << YAML::Key << "FontDirectory" << YAML::Value << m_project->m_config.fontPath;
//      out << YAML::Key << "SceneDirectory" << YAML::Value << m_project->m_config.scenePath;
//      out << YAML::Key << "TextureDirectory" << YAML::Value << m_project->m_config.texturePath;
//      out << YAML::Key << "MeshSourceDirectory" << YAML::Value << m_project->m_config.meshSourcePath;
//      out << YAML::Key << "MaterialPath" << YAML::Value << m_project->m_config.materialPath;
//      out << YAML::Key << "AssetRegistry" << YAML::Value << m_project->m_config.assetRegistryPath;
//      out << YAML::Key << "StartScene" << YAML::Value << m_project->m_config.startScene;
//      out << YAML::Key << "AutoSave" << YAML::Value << m_project->m_config.enableAutoSave;
//      out << YAML::Key << "AutoSaveInterval" << YAML::Value << m_project->m_config.autoSaveIntervalSeconds;
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
