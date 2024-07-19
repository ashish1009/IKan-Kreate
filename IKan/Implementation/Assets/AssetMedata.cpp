//
//  AssetMedata.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "AssetMedata.hpp"

namespace IKan
{
  bool AssetMetadata::IsValid() const
  {
    return handle != 0 and !isMemoryAsset;
  }
} // namespace IKan
