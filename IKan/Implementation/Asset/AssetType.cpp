//
//  AssetType.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "AssetType.hpp"

namespace IKan
{
  namespace AssetUtils
  {
    AssetType AssetTypeFromString(const std::string& assetType)
    {
      if (assetType == "None")                return AssetType::None;
      if (assetType == "Texture")             return AssetType::Texture;
      if (assetType == "Font")                return AssetType::Font;
      if (assetType == "Scene")               return AssetType::Scene;
      IK_ASSERT(false, "Unknown Asset Type");
      return AssetType::None;
    }
    
    const char* AssetTypeToString(AssetType assetType)
    {
      switch (assetType)
      {
        case AssetType::None:                  return "None";
        case AssetType::Texture:               return "Texture";
        case AssetType::Font:                  return "Font";
        case AssetType::Scene:                 return "Scene";
      }
      IK_ASSERT(false, "Unknown Asset Type");
      return "None";
    }
  } // namespace AssetUtils
} // namespace IKan
