//
//  AssetManager.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Assets/AssetRegistry.hpp"
#include "Assets/AssetImporter.hpp"

namespace IKan
{
  /// This class manages the assets
  class AssetManager
  {
  public:
    /// This function Initialises the asset manager
    static void Initialize();
    /// This function Shutdown the asset manager
    static void Shutdown();

    DELETE_ALL_CONSTRUCTORS(AssetManager);
    
  private:
    // Member Variables ---------------------------------------------------------------------------------------------
    inline static AssetRegistry s_assetRegistry;
    inline static std::unordered_map<AssetHandle, Ref<Asset>> s_memoryAssets;
    inline static std::unordered_map<AssetHandle, Ref<Asset>> s_loadedAssets;
  };
} // namespace IKan
