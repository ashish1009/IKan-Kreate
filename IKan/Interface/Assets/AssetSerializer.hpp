//
//  AssetSerializer.hpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#pragma once

#include "Assets/AssetMetadata.hpp"

namespace IKan
{
  /// This class is the interface to serialis the asset. Implementation for each asset is required
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

  class ImageSerializer : public AssetSerializer
  {
  public:
    /// @see: AssetSerializer
    virtual void Serialize([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] const Ref<Asset>& asset) const override{ IK_ASSERT(false);}
    /// @see: AssetSerializer
    virtual bool TryLoadData([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] Ref<Asset>& asset) const override{ IK_ASSERT(false); return false; }
  };
  
  class FontSerializer : public AssetSerializer
  {
  public:
    /// @see: AssetSerializer
    virtual void Serialize([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] const Ref<Asset>& asset) const override {IK_ASSERT(false);}
    /// @see: AssetSerializer
    virtual bool TryLoadData([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] Ref<Asset>& asset) const override{ IK_ASSERT(false); return false; }
  };
  
  class MeshSerializer : public AssetSerializer
  {
  public:
    /// @see: AssetSerializer
    virtual void Serialize([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] const Ref<Asset>& asset) const override{ IK_ASSERT(false);}
    /// @see: AssetSerializer
    virtual bool TryLoadData([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] Ref<Asset>& asset) const override{ IK_ASSERT(false); return false; }
  };
  
  class MaterialSerializer : public AssetSerializer
  {
  public:
    /// @see: AssetSerializer
    virtual void Serialize([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] const Ref<Asset>& asset) const override{ IK_ASSERT(false);}
    /// @see: AssetSerializer
    virtual bool TryLoadData([[maybe_unused]] const AssetMetadata& metadata, [[maybe_unused]] Ref<Asset>& asset) const override{ IK_ASSERT(false); return false; }
  };
} // namespace IKan
