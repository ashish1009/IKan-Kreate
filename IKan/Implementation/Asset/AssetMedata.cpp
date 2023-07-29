//
//  AssetMedata.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetMedata.hpp"

namespace IKan
{
  bool AssetMetadata::IsValid() const
  {
    return handle != 0 and !isMemoryAsset;
  }
} // namespace IKan
