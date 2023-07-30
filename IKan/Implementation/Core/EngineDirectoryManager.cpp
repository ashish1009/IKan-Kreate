//
//  EngineDirectoryManager.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "EngineDirectoryManager.hpp"

namespace IKan
{
  std::string CoreDirectory::GetEngineInstallPath()
  {
    return s_engineBasePath;
  }
  
  std::string CoreDirectory::GetAssetBasePath()
  {
    return s_engineBasePath + "/Assets/";
  }
  
  std::string CoreDirectory::RelativeEnginePath(const std::string &path)
  {
    return s_engineBasePath + path;
  }
  
  std::string CoreDirectory::RelativeAssetPath(const std::string &assetPath)
  {
    return GetAssetBasePath() + assetPath;
  }
  
  void CoreDirectory::SetPath(const std::string &enginePath)
  {
    IK_LOG_INFO(LogModule::IKan, "Engine is Instal path is {0}", Utils::FileSystem::Absolute(enginePath.c_str()));
    
    bool exist = Utils::FileSystem::Exists(enginePath);
    bool assets = Utils::FileSystem::Exists(enginePath + "/Assets");
    bool interface = Utils::FileSystem::Exists(enginePath + "/Interface");
    bool implementation = Utils::FileSystem::Exists(enginePath + "/Implementation");
    bool vendors = Utils::FileSystem::Exists(enginePath + "/Vendors");
    
    IK_ASSERT(exist and assets and interface and implementation and vendors, "Invalid Engine Directory");
    s_engineBasePath = enginePath;
  }
} // namespace IKan
