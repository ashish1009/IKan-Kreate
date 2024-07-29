//
//  AssetType.cpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#include "AssetType.hpp"

namespace IKan
{
  namespace AssetUtils
  {
    AssetType AssetTypeFromString(const std::string& assetType)
    {
      if (assetType == "None")                return AssetType::Invalid;
      if (assetType == "Image")               return AssetType::Image;
      if (assetType == "Font")                return AssetType::Font;
      if (assetType == "Mesh")                return AssetType::Mesh;
      if (assetType == "Material")            return AssetType::Material;
      if (assetType == "Scene")               return AssetType::Scene;
      if (assetType == "Physics")             return AssetType::Physics;
      if (assetType == "Prefab")              return AssetType::Prefab;
      IK_ASSERT(false, "Unknown Asset Type");
      return AssetType::Invalid;
    }
    
    const char* AssetTypeToString(AssetType assetType)
    {
      switch (assetType)
      {
        case AssetType::Invalid:      return "Invalid";
        case AssetType::Image:        return "Image";
        case AssetType::Font:         return "Font";
        case AssetType::Mesh:         return "Mesh";
        case AssetType::Material:     return "Material";
        case AssetType::Scene:        return "Scene";
        case AssetType::Physics:      return "Physics";
        case AssetType::Prefab:       return "Prefab";
      }
      IK_ASSERT(false, "Unknown Asset Type");
      return "None";
    }
  } // namespace AssetUtils
} // namespace IKan
