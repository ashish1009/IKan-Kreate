//
//  AssetImporter.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Assets/AssetSerializer.hpp"

namespace IKan
{
  /// This class helps to import the asset
  class AssetImporter
  {
  public:
    /// This function initialise the Asset importer
    static void Initialize();
    /// This function shuts down the Asset importer
    static void Shutdown();
    
    /// This function serialise the asset
    /// - Parameters:
    ///   - metadata: Asset Metadata
    ///   - asset: Asset Instance
    static void Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset);
    /// This function serialise the asset weth asseet only
    /// - Parameter asset: Asset instance
    static void Serialize(const Ref<Asset>& asset);
    /// This function try to load the asset
    /// - Parameters:
    ///   - metadata: Asset meta data
    ///   - asset: Asset instance
    static bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset);

  private:
    inline static std::unordered_map<AssetType, Scope<AssetSerializer>> s_serializers;
  };
} // namespace IKan
