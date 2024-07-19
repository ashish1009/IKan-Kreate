//
//  AssetMedata.hpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#pragma once

#include "Assets/Asset.hpp"

namespace IKan
{
  /// This structure store the metadata for Asset
  struct AssetMetadata
  {
    AssetHandle handle {0};
    AssetType type {AssetType::Invalid};
    
    std::filesystem::path filePath {}; // Relative to asset common directory
    
    bool isDataLoaded {false};
    bool isMemoryAsset {false};

    /// This function checks is this asset metadata is valid
    bool IsValid() const;
  };
} // namespace IKan
