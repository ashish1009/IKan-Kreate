//
//  AssetManager.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "AssetManager.hpp"

#include <yaml-cpp/yaml.h>

#include "Project/Project.hpp"

namespace IKan
{
  static AssetMetadata NullMetadata;
  
  void AssetManager::Initialize()
  {
    IK_PROFILE()
    IK_LOG_INFO(LogModule::Asset, "Initializing current Asset Manager");
    
    // Clear previous registry
    s_assetRegistry.Clear();
    
    // Intialize asset importers
    AssetImporter::Initialize();
  }
  
  void AssetManager::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Asset, "Shutting down current Asset Manager");
    
    AssetImporter::Shutdown();
  }
} // namespace IKan
