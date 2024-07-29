//
//  AssetManager.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Assets/AssetRegistry.hpp"
#include "Assets/AssetImporter.hpp"

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
    
    /// This function returns the Asset handle from file path
    /// - Parameter metadata: Asset Meta data
    static AssetHandle GetAssetHandleFromFilePath(const std::filesystem::path& filepath);
    
    /// This function returns the asset meta data from handle
    static const AssetMetadata& GetMetadata(AssetHandle handle);
    /// This function returns the asset meta data from file path
    /// - Parameter filepath: Asset file path
    static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);
    
    /// This function checks the asset is memory asset
    /// - Parameter handle: Asset handle
    static bool IsMemoryAsset(AssetHandle handle);
    /// This function checks the asset is loaded asset
    /// - Parameter handle: Asset handle
    static bool IsLoadedAsset(AssetHandle handle);
    /// This functuion checks is asset handle is valid
    /// - Parameter assetHandle: Asset handle
    static bool IsAssetHandleValid(AssetHandle assetHandle);
    
    /// This function handles asset rename
    /// - Parameters:
    ///   - assetHandle: Asset handle
    ///   - newFilePath: New name
    static void OnAssetRenamed(AssetHandle assetHandle, const std::filesystem::path& newFilePath);
    /// This function handles asset movement
    /// - Parameters:
    ///   - assetHandle: Asset handle
    ///   - newFilePath: new location
    static void OnAssetMoved(AssetHandle assetHandle, const std::filesystem::path& destinationPath);
    /// This function handles asset deletion
    /// - Parameters:
    ///   - assetHandle: Asset handle
    static void OnAssetDeleted(AssetHandle assetHandle);
    
    /// This funcion reload the asset registry
    /// - Parameter assetHandle: Asset handle for base
    static bool ReloadData(AssetHandle assetHandle);
    
    /// This function checks is file exist for metadata
    /// - Parameter metadata: metadata handle
    static bool FileExists(AssetMetadata& metadata);
    
    /// This function returns the asset registry
    static const AssetRegistry& GetAssetRegistry();
    
    DELETE_ALL_CONSTRUCTORS(AssetManager);
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This funciton loads the asset reguistry file path and update the registry data in manager
    static void LoadAssetRegistry();
    /// This function reload the assets stores in registry
    static void ReloadAssets();
    
    /// This function process all the directory recursively
    /// - Parameter baseDirectory: start directory
    static void ProcessDirectory(const std::filesystem::path& baseDirectory);
    /// This funciton writes the registry data to file
    static void WriteRegistryToFile();

    /// This function returns the metada from handle
    /// - Parameter handle: Asset handle
    static AssetMetadata& GetMetadataInternal(AssetHandle handle);

    // Member Variables ---------------------------------------------------------------------------------------------
    inline static AssetRegistry s_assetRegistry;
    inline static std::unordered_map<AssetHandle, Ref<Asset>> s_memoryAssets;
    inline static std::unordered_map<AssetHandle, Ref<Asset>> s_loadedAssets;
  };
} // namespace IKan
