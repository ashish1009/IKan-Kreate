//
//  AssetMetadata.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

#include "Asset/Asset.hpp"

namespace IKan
{
  /// This structure stores the Asset Data required by asset manager
  struct AssetMetadata
  {
    AssetHandle handle = 0;
    AssetType type;
    
    std::filesystem::path filePath; // Relative file path from asset directory
    bool isDataLoaded = false;
    bool isMemoryAsset = false;
    
    /// This function checks is this metadata is valid
    bool IsValid() const;
  };
} // namespace IKan
