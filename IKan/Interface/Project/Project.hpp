//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 15/02/24.
//

#pragma once

namespace IKan
{
  struct ProjectConfig
  {
    // General
    std::string name = "New Project";

    // Asset Data
    std::filesystem::path assetDirectory;
    std::filesystem::path assetRegistryPath;
    std::filesystem::path fontPath;
    std::filesystem::path meshPath;
    std::filesystem::path materialPath;
    std::filesystem::path scenePath;
    std::filesystem::path texturePath;
    
    // Scene Serialize
    std::string startScene;
    bool enableAutoSave = false;
    int32_t autoSaveIntervalSeconds = 300;

    // Not serialized
    std::string projectFileName;
    std::filesystem::path projectDirectory;
  };
} // namespace IKan
