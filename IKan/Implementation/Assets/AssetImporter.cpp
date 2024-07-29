//
//  AssetImporter.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "AssetImporter.hpp"

namespace IKan
{
  void AssetImporter::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Asset, "Initializing Asset Importer");
    s_serializers.clear();
    s_serializers[AssetType::Image] = CreateScope<ImageSerializer>();
    s_serializers[AssetType::Font] = CreateScope<FontSerializer>();
    s_serializers[AssetType::Mesh] = CreateScope<MeshSerializer>();
    s_serializers[AssetType::Material] = CreateScope<MaterialSerializer>();
  }
  
  void AssetImporter::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Asset, "Initializing Asset Importer");
    s_serializers.clear();
  }
} // namespace IKan
