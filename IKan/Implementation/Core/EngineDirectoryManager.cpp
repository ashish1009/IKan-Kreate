//
//  EngineDirectoryManager.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "EngineDirectoryManager.hpp"

namespace IKan
{
  std::filesystem::path CoreResourcesPath::GetEngineResourcesPath()
  {
    return s_engineResourcesPath;
  }
  
  
  std::filesystem::path CoreResourcesPath::RelativeResourcePath(const std::filesystem::path &assetPath)
  {
    return s_engineResourcesPath / assetPath;
  }
  
  void CoreResourcesPath::SetPath(const std::filesystem::path &enginePath)
  {
    std::filesystem::path absPath = Utils::FileSystem::IKanAbsolute(enginePath.c_str());
    IK_LOG_TRACE(LogModule::IKan, "Engine is Instal path is {0}", absPath.string().c_str());
    
    bool exist = Utils::FileSystem::Exists(absPath);
    bool shaders = Utils::FileSystem::Exists(absPath / "Shaders");
    bool textures = Utils::FileSystem::Exists(absPath / "Textures");
    bool fonts = Utils::FileSystem::Exists(absPath / "Fonts");
    
    IK_ASSERT(exist and fonts and shaders and textures, "Invalid Engine Directory");
    s_engineResourcesPath = enginePath;
  }
} // namespace IKan
