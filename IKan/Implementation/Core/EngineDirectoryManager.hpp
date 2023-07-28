//
//  EngineDirectoryManager.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Utils/FileSystem.hpp"

namespace IKan
{
#define CoreAssetPath CoreDirectory::RelativeAssetPath
#define EnginePath CoreDirectory::RelativeEnginePath
  
  class Application;
  
  /// Stores the Engine directory path
  struct CoreDirectory
  {
  public:
    /// This function returns the core engine base path
    /// - Important: To be setup via application specificaion
    static std::string GetEngineInstallPath();
    /// This function returns the Core Asset Base path
    /// - Important: Returned path is relative to .exce and upto the folder name of 'core_assets'
    static std::string GetAssetBasePath();
    
    /// This function returns the path relative to the executable form the path relative to engine
    /// - Parameter path: Path relative to workspace folder:
    static std::string RelativeEnginePath(const std::string& path);
    /// This function returns the path relative to the executable form the path relative to Core asset path
    /// - Parameter asset_path: Aasset path relative to core_asset folder:
    static std::string RelativeAssetPath(const std::string& assetPath);
    
    MAKE_PURE_STATIC(CoreDirectory);
    friend class Application;

  private:
    // Private APIs --------------------------------------------------------------------------------------------------
    static void SetPath(const std::string& enginePath);
    
    // Direcotry path of IKan Engine relative to Executable ----------------------------------------------------------
    inline static std::string s_engineBasePath;
  };  
} // namespace IKan
