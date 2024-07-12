//
//  CoreAssetPath.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

namespace IKan
{
#define CoreAsset CoreAssetPath::RelativeResourcePath
  
  /// This stucture stores the Engine directory path
  struct CoreAssetPath
  {
  public:
    /// This function returns the path relative to the executable form the path relative to Core asset path
    /// - Parameter assetRelativePath: Aasset path relative to assetRelativePath folder:
    static std::filesystem::path RelativeResourcePath(const std::filesystem::path& assetRelativePath = "")
    {
      return s_assetPath / assetRelativePath;
    }
    
    DELETE_ALL_CONSTRUCTORS(CoreAssetPath);
    
  private:
    inline static const std::filesystem::path s_assetPath {"../../../IKan/Assets"};
  };
} // namespace IKan
