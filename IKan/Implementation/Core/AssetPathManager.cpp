//
//  AssetPathManager.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "AssetPathManager.hpp"

namespace IKan
{
  std::filesystem::path CoreAssetPath::RelativeResourcePath(const std::filesystem::path &assetPath)
  {
    return s_assetPath / assetPath;
  }
  
  void CoreAssetPath::SetPath(const std::filesystem::path &assetPath)
  {
    std::filesystem::path absPath = Utils::FileSystem::IKanAbsolute(assetPath.c_str());
    IK_LOG_INFO(LogModule::IKan, "Core Asset path is {0}", absPath.string().c_str());
    
    bool exist = std::filesystem::exists(absPath);
    bool shaders = std::filesystem::exists(absPath / "Shaders");
    bool textures = std::filesystem::exists(absPath / "Textures");
    bool fonts = std::filesystem::exists(absPath / "Fonts");
    bool meshes = std::filesystem::exists(absPath / "Meshes");

    IK_ASSERT(exist and fonts and shaders and textures and meshes, "Invalid Engine Directory");
    s_assetPath = assetPath;
  }
} // namespace IKan
