//
//  Project.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

namespace IKan
{
  class Project
  {
  public:
    /// This structure stores the project confguration
    struct Config
    {
      std::string name = "New_Project";
      std::string assetDirectory;
      std::string assetRegistryPath;
      
      std::string startScene;
      bool enableAutoSave = false;
      int32_t autoSaveIntervalSeconds = 300;
      
      // Not serialized
      std::string projectFileName;
      std::string projectDirectory;
    };
  };
} // namespace IKan
