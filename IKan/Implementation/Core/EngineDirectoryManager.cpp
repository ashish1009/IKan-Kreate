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
    IK_LOG_INFO(LogModule::IKan, "Engine is Instal path is {0}", Utils::FileSystem::IKanAbsolute(enginePath.c_str()));
    
    bool exist = Utils::FileSystem::Exists(enginePath);
    bool shaders = Utils::FileSystem::Exists(enginePath / "Shaders");
    bool textures = Utils::FileSystem::Exists(enginePath / "Textures");
    bool fonts = Utils::FileSystem::Exists(enginePath / "Fonts");
    
    IK_ASSERT(exist and fonts and shaders and textures, "Invalid Engine Directory");
    s_engineResourcesPath = enginePath;
  }
} // namespace IKan
