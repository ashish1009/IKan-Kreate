//
//  AssetType.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

namespace IKan
{
  /// Flag for action in assets
  enum class AssetFlag : uint16_t
  {
    None = 0,
    Missing = BIT(0),
    Invalid = BIT(1)
  };
  
  /// This enum stores the type of asset
  enum class AssetType : uint16_t
  {
    None = 0,
    Texture,
    Font,
    Scene
  };
} // namespace IKan
