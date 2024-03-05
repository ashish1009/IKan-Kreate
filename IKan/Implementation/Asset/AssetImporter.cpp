//
//  AssetImporter.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "AssetImporter.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  void AssetImporter::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Asset, "Initializing Asset Importer");
    s_serializers.clear();
    s_serializers[AssetType::Image] = CreateScope<ImageSerializer>();
    s_serializers[AssetType::Font] = CreateScope<FontSerializer>();
    s_serializers[AssetType::Scene] = CreateScope<SceneAssetSerializer>();
    s_serializers[AssetType::Mesh] = CreateScope<MeshSerializer>();
    s_serializers[AssetType::Material] = CreateScope<MaterialSerializer>();
    s_serializers[AssetType::Prefab] = CreateScope<PrefabSerializer>();
    s_serializers[AssetType::NodeGraph] = CreateScope<NodeGraphSerializer>();
  }
  
  void AssetImporter::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset)
  {
    IK_PROFILE();
    if (s_serializers.find(metadata.type) == s_serializers.end())
    {
      IK_LOG_WARN(LogModule::Asset, "There's currently no importer for assets of type {0}", metadata.filePath.stem().string());
      return;
    }
    s_serializers[asset->GetAssetType()]->Serialize(metadata, asset);
  }
  
  void AssetImporter::Serialize(const Ref<Asset>& asset)
  {
    IK_PROFILE();
    const AssetMetadata& metadata = AssetManager::GetMetadata(asset->handle);
    Serialize(metadata, asset);
  }
  
  bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
  {
    IK_PROFILE();
    if (s_serializers.find(metadata.type) == s_serializers.end())
    {
      IK_LOG_WARN(LogModule::Asset, "There's currently no importer for assets of type {0}", metadata.filePath.stem().string());
      return false;
    }
    return s_serializers[metadata.type]->TryLoadData(metadata, asset);
  }
} // namespace IKan
