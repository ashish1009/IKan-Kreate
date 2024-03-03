//
//  AssetType.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

namespace IKan
{
  static const std::string MaterialExtension {".ikmat"};
  static const std::string ProjectExtension {".ikproj"};
  static const std::string SceneExtension {".ikscene"};

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
    Invalid = 0,
    Image,
    Font,
    Scene,
    Mesh,
    Material,
    Prefab,
  };
  
  /// Map to store the asset with its Extension
  inline static std::unordered_map<std::string /*Extesntion*/, AssetType> s_assetExtensionMap =
  {
    // Image
    { ".png", AssetType::Image },
    { ".jpg", AssetType::Image },
    { ".jpeg", AssetType::Image },
    { ".hdr", AssetType::Image },
    
    // Scene
    { ".ikscene", AssetType::Scene },
    { ".ikscene.auto", AssetType::Scene },
    
    // Fonts
    { ".ttf", AssetType::Font },
    
    // Mesh source
    { ".fbx", AssetType::Mesh },
    { ".obj", AssetType::Mesh },
    
    // Material
    { ".ikmat", AssetType::Material } ,
    
    // Prefab
    { ".ikprefab", AssetType::Prefab}
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
