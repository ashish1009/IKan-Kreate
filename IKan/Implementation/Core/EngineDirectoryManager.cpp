//
//  EngineDirectoryManager.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "EngineDirectoryManager.hpp"

namespace IKan
{
  std::filesystem::path CoreDirectory::GetEngineInstallPath()
  {
    return s_engineBasePath;
  }
  
  std::filesystem::path CoreDirectory::GetAssetBasePath()
  {
    return s_engineBasePath / "Assets/";
  }
  
  std::filesystem::path CoreDirectory::RelativeEnginePath(const std::filesystem::path &path)
  {
    return s_engineBasePath / path;
  }
  
  std::filesystem::path CoreDirectory::RelativeAssetPath(const std::filesystem::path &assetPath)
  {
    return GetAssetBasePath() / assetPath;
  }
  
  void CoreDirectory::SetPath(const std::filesystem::path &enginePath)
  {
    IK_LOG_INFO(LogModule::IKan, "Engine is Instal path is {0}", Utils::FileSystem::KreatorAbsolute(enginePath.c_str()));
    
    bool exist = Utils::FileSystem::Exists(enginePath);
    bool assets = Utils::FileSystem::Exists(enginePath / "Assets");
    bool interface = Utils::FileSystem::Exists(enginePath / "Interface");
    bool implementation = Utils::FileSystem::Exists(enginePath / "Implementation");
    bool vendors = Utils::FileSystem::Exists(enginePath / "Vendors");
    
    IK_ASSERT(exist and assets and interface and implementation and vendors, "Invalid Engine Directory");
    s_engineBasePath = enginePath;
  }
} // namespace IKan
