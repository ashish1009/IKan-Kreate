//
//  AssetType.hpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

namespace IKan
{
  static const std::string MaterialExtension {".ikmat"};
  static const std::string PhysicsExtension {".ikphysics"};
  static const std::string PrefabExtension {".ikprefab"};
  static const std::string SceneExtension {".ikscene"};

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
    Scene,
    Physics,
    Prefab
  };
  
  inline static std::unordered_map<std::string, AssetType> s_assetExtensionMap =
  {
    // Image
    { ".png",  AssetType::Image },
    { ".jpg",  AssetType::Image },
    { ".jpeg", AssetType::Image },
    { ".hdr",  AssetType::Image },
    
    // Fonts
    { ".ttf", AssetType::Font },
    
    // Mesh source
    { ".fbx", AssetType::Mesh },
    { ".obj", AssetType::Mesh },
    
    // Material
    { MaterialExtension, AssetType::Material },

    // Physics
    { PhysicsExtension, AssetType::Physics },

    // Prefab
    { PrefabExtension, AssetType::Prefab },

    // Scene
    { SceneExtension, AssetType::Scene },
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
