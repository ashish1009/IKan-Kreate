//
//  MaterialAsset.hpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#pragma once

#include "Asset/Asset.hpp"

namespace IKan
{
  class MaterialAsset : public Asset
  {
  public:
    ASSET_TYPE(Material);
  };
  
  class MaterialTable
  {
  public:
    std::map<uint32_t, Ref<MaterialAsset>> m_materials;
    uint32_t m_materialCount;
  };
} // namespace IKan
