//
//  ProjectConfig.hpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#pragma once

#include "Scene/Scene.hpp"

namespace IKan
{
  /// This structure stores the configuration of project
  struct ProjectConfig
  {
    // General
    std::string name {"New Project"};
    
    // Asset Data
    std::filesystem::path assetDirectory;
    std::filesystem::path assetRegistryPath;
    
    std::filesystem::path texturePath;
    std::filesystem::path fontPath;
    std::filesystem::path scenePath;
    std::filesystem::path meshPath;
    std::filesystem::path materialPath;
    std::filesystem::path physicsPath;
    std::filesystem::path prefabPath;
    
    // Scene Serialize
    SceneType sceneType {SceneType::None};
    std::filesystem::path startScene{};
    bool enableAutoSave {false};
    int32_t autoSaveIntervalSeconds {300};
    
    // Not serialized
    std::string projectFileName;
    std::filesystem::path projectDirectory;
  };
} // namespace IKan
