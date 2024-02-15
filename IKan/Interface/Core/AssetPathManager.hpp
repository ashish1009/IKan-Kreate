//
//  AssetPathManager.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
#define CoreAsset CoreAssetPath::RelativeResourcePath
  
  /// Stores the Engine directory path
  struct CoreAssetPath
  {
  public:
    /// This function returns the path relative to the executable form the path relative to Core asset path
    /// - Parameter assetPath: Aasset path relative to core_asset folder:
    static std::filesystem::path RelativeResourcePath(const std::filesystem::path& assetPath);
    
    DELETE_ALL_CONSTRUCTORS(CoreAssetPath);
    friend class Application;
    
  private:
    /// This function validate the asset folder path and set the path
    /// - Parameter assetPath: core asset folder path
    static void SetPath(const std::filesystem::path &assetPath);
    
    inline static std::filesystem::path s_assetPath {};
  };
} // namespace IKan
