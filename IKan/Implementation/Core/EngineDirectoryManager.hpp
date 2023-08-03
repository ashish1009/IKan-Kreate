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
#define CoreAssetPath CoreResourcesPath::RelativeResourcePath
  
  class Application;
  
  /// Stores the Engine directory path
  struct CoreResourcesPath
  {
  public:
    /// This function returns the core engine base path
    /// - Important: To be setup via application specificaion
    static std::filesystem::path GetEngineResourcesPath();
    
    /// This function returns the path relative to the executable form the path relative to Core asset path
    /// - Parameter asset_path: Aasset path relative to core_asset folder:
    static std::filesystem::path RelativeResourcePath(const std::filesystem::path& assetPath);
    
    MAKE_PURE_STATIC(CoreResourcesPath);
    friend class Application;

  private:
    // Private APIs --------------------------------------------------------------------------------------------------
    static void SetPath(const std::filesystem::path& enginePath);
    
    // Direcotry path of IKan Engine relative to Executable ----------------------------------------------------------
    inline static std::filesystem::path s_engineResourcesPath;
  };  
} // namespace IKan
