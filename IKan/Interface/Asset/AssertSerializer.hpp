//
//  AssertSerializer.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

#include "Asset/AssetMetadata.hpp"

namespace IKan
{
  /// This class serialises the asset
  class AssetSerializer
  {
  public:
    virtual ~AssetSerializer() = default;
    
    /// This function serialise the asset
    /// - Parameters:
    ///   - metadata: Asset Metadata
    ///   - asset: Asset Instance
    virtual void Serialize(const AssetMetadata& metadata, const Ref<Asset>& asset) const = 0;
    /// This function try to load the asset
    /// - Parameters:
    ///   - metadata: Asset meta data
    ///   - asset: Asset instance
    virtual bool TryLoadData(const AssetMetadata& metadata, Ref<Asset>& asset) const = 0;
  };
} // namespace IKan
