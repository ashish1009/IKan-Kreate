//
//  AssetImporter.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetImporter.hpp"

namespace IKan
{
  void AssetImporter::Initialize()
  {
    s_serializers.clear();
    s_serializers[AssetType::Image] = CreateScope<ImageSerializer>();
    s_serializers[AssetType::Font] = CreateScope<FontSerializer>();
    s_serializers[AssetType::Scene] = CreateScope<SceneAssetSerializer>();
    s_serializers[AssetType::MeshSource] = CreateScope<MeshSourceSerializer>();
  }
  
  void AssetImporter::Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset)
  {
    IK_ASSERT(false);
  }
  
  void AssetImporter::Serialize(const Ref<Asset>& asset)
  {
    IK_ASSERT(false);
  }
  
  bool AssetImporter::TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset)
  {
    if (s_serializers.find(metadata.type) == s_serializers.end())
    {
      IK_LOG_WARN(LogModule::Asset, "There's currently no importer for assets of type {0}", metadata.filePath.stem().string());
      return false;
    }
    return s_serializers[metadata.type]->TryLoadData(metadata, asset);
  }
} // namespace IKan
