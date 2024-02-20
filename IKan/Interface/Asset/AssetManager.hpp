//
//  AssetManager.hpp
//  IKan
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

#include "Asset/AssetRegistry.hpp"
#include "Asset/AssetImporter.hpp"

namespace IKan
{
  /// This class manages the assets
  class AssetManager
  {
  public:
    /// This function Initialises the asset manager
    static void Initialize();
    /// This function Shutdown the asset manager
    static void Shutdown();
    
    /// This function imports the assets as store in registry
    /// - Parameter filepath: Asset file path
    static AssetHandle ImportAsset(const std::filesystem::path& filepath);

    /// This function returns the relative path from asset base path
    /// - Parameter filepath: file path
    static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);
    /// This function returns the asset type from file path
    /// - Parameter path: File path
    static AssetType GetAssetTypeFromPath(const std::filesystem::path& path);
    /// This function returns the asset type from file Extesion
    /// - Parameter extension: Extesion
    static AssetType GetAssetTypeFromExtension(const std::string& extension);
    /// This function returns the system file path
    /// - Parameter metadata: Asset Meta data
    static std::filesystem::path GetFileSystemPath(const AssetMetadata& metadata);

  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This funciton loads the asset reguistry file path and update the registry data in manager
    static void LoadAssetRegistry();
    /// This function reload the assets stores in registry
    static void ReloadAssets();

    /// This function process all the directory recursively
    /// - Parameter baseDirectoru: start directory
    static void ProcessDirectory(const std::string& baseDirectoru);
    /// This funciton writes the registry data to file
    static void WriteRegistryToFile();

    // Member Variables ---------------------------------------------------------------------------------------------
    inline static AssetRegistry s_assetRegistry;
  };
} // namespace IKan
