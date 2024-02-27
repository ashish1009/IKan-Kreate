//
//  AssetType.cpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
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
      if (assetType == "Scene")               return AssetType::Scene;
      if (assetType == "Mesh")                return AssetType::Mesh;
      if (assetType == "Material")            return AssetType::Material;
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
        case AssetType::Scene:        return "Scene";
        case AssetType::Mesh:         return "Mesh";
        case AssetType::Material:     return "Material";
      }
      IK_ASSERT(false, "Unknown Asset Type");
      return "None";
    }
  } // namespace AssetUtils
} // namespace IKan
