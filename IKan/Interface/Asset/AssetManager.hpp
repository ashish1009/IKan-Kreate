//
//  AssetManager.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

#include "Asset/AssetRegistry.hpp"
#include "Asset/AssetImporter.hpp"

namespace IKan
{
  /// This class manages the assets
  class AssetManager
  {
  public:
    /// This function Initialises the asset manager
    static void Initialize();
    /// This function Shutdown the asset manager
    static void Shutdown();
  };
} // namespace IKan
