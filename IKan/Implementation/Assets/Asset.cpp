//
//  Asset.cpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#include "Asset.hpp"

namespace IKan
{
  AssetType Asset::GetStaticType()
  {
    return AssetType::Invalid;
  }
  
  bool Asset::IsValid() const
  {
    return ((flags & (uint16_t)AssetFlag::Missing) | (flags & (uint16_t)AssetFlag::Invalid)) == 0;
  }
  
  bool Asset::operator==(const Asset& other) const
  {
    return handle == other.handle;
  }
  
  bool Asset::operator!=(const Asset& other) const
  {
    return !(*this == other);
  }
  
  bool Asset::IsFlagSet(AssetFlag flag) const
  {
    return (uint16_t)flag & flags;
  }
  void Asset::SetFlag(AssetFlag flag, bool value)
  {
    if (value)
    {
      flags |= (uint16_t)flag;
    }
    else
    {
      flags &= ~(uint16_t)flag;
    }
  }
} // namespace IKan
