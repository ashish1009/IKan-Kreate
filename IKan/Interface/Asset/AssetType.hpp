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
  
  /// Map to store the asset with its Extension
  inline static std::unordered_map<std::string, AssetType> s_assetExtensionMap =
  {
    // Textures
    { ".png", AssetType::Texture },
    { ".jpg", AssetType::Texture },
    { ".jpeg", AssetType::Texture },
    
    // Scene
    { ".ikscene", AssetType::Scene },
    
    // Fonts
    { ".ttf", AssetType::Font },
  };

  namespace AssetUtils
  {
    /// This function return the asset type from string
    /// - Parameter assetType: asset type as string
    AssetType AssetTypeFromString(const std::string& assetType);
    /// This function retutns the asset type as string
    /// - Parameter assetType: Asset type
    const char* AssetTypeToString(AssetType assetType);
  } // namespace AssetUtils

} // namespace IKan
