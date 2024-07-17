//
//  Asset.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

#include "Core/UUID.hpp"
#include "Assets/AssetType.hpp"

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

  /// This structure is the base class of Asset. Assets used by Engine / Editor should derived from this class for easy access
  struct Asset
  {
    AssetHandle handle {0};
    uint16_t flags = (uint16_t)AssetFlag::None;

    // Virtal APIs ---------------------------------------------------------------------------------------------------
    /// Default virtual destructor
    virtual ~Asset() = default;
    
    /// This function returns the Asset type. Override in Derived class
    virtual AssetType GetAssetType() const = 0;
    /// This function compare tow assets
    /// - Parameter other: Other Asset
    virtual bool operator==(const Asset& other) const;
    /// This function compare tow assets
    /// - Parameter other: Other Asset
    virtual bool operator!=(const Asset& other) const;

    // Fundamental APIs ----------------------------------------------------------------------------------------------
    /// This function checks is asset valid
    bool IsValid() const;
    /// This function checks the flag is set or not
    /// - Parameter flag: asset flag
    bool IsFlagSet(AssetFlag flag) const;
    /// This function sets the flag to asset
    /// - Parameter flag: asset flag
    void SetFlag(AssetFlag flag, bool value = true);
    
    // Static APIs ---------------------------------------------------------------------------------------------------
    /// This static API returns the type of Asset as same for all base class
    static AssetType GetStaticType();
  };
} // namespace IKan
