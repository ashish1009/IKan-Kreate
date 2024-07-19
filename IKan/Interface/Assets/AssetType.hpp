//
//  AssetType.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

namespace IKan
{
  /// This enum stores flag for actions in assets
  enum class AssetFlag : uint16_t
  {
    None = 0,
    Invalid = BIT(0),
    Missing = BIT(1),
  };
  
  /// This enum stores the supported asset type of engine
  enum class AssetType : uint8_t
  {
    Invalid,
    Image,
    Font,
    Mesh,
    Material,
  };
} // namespace IKan
