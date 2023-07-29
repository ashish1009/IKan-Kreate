//
//  Asset.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Core/UUID.hpp"
#include "Asset/AssetType.hpp"

namespace IKan
{
#define ASSET_TYPE(type) \
static AssetType GetStaticType() { \
return AssetType::type; \
} \
virtual AssetType GetAssetType() const override { \
return GetStaticType(); \
} \

  using AssetHandle = UUID;
  
  /// This is the base class for all assets
  class Asset
  {
  public:
    AssetHandle handle = 0;
    uint16_t flags = (uint16_t)AssetFlag::None;
    
    virtual ~Asset() = default;
    
    /// This static API returns the type of Asset as same for all base class
    static AssetType GetStaticType();
    
    /// This function returns the Asset type. Override in Derived class
    virtual AssetType GetAssetType() const = 0;
    /// This function compare tow assets
    /// - Parameter other: Other Asset
    virtual bool operator==(const Asset& other) const;
    /// This function compare tow assets
    /// - Parameter other: Other Asset
    virtual bool operator!=(const Asset& other) const;
    
    /// This function checks is asset valid
    bool IsValid() const;
    /// This function checks the flag is set or not
    /// - Parameter flag: asset flag
    bool IsFlagSet(AssetFlag flag) const;
    /// This function sets the flag to asset
    /// - Parameter flag: asset flag
    void SetFlag(AssetFlag flag, bool value = true);
    
    /// This function should be present in Derived class
    static Ref<Asset> Create() {IK_ASSERT(false, "Implement in Derived Asset");}
  };
  
} // namespace IKan
