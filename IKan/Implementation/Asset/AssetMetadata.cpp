//
//  AssetMetadata.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#include "AssetMetadata.hpp"

namespace IKan
{
  bool AssetMetadata::IsValid() const
  {
    return handle != 0 and !isMemoryAsset;
  }
} // namespace IKan

