//
//  AssetManager.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
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
    static void Init();
    /// This function Shutdown the asset manager
    static void Shutdown();
    
    /// This function returns the relative path from asset base path
    /// - Parameter filepath: file path
    static std::filesystem::path GetRelativePath(const std::filesystem::path& filepath);
    /// This function returns the system file path
    /// - Parameter metadata: Asset Meta data
    static std::filesystem::path GetFileSystemPath(const AssetMetadata& metadata);
    /// This function returns the system file path as string
    /// - Parameter metadata: Asset Meta data
    static std::string GetFileSystemPathString(const AssetMetadata& metadata);
    
    /// This function returns the Asset handle from file path
    /// - Parameter metadata: Asset Meta data
    static AssetHandle GetAssetHandleFromFilePath(const std::filesystem::path& filepath);
    
    /// This function returns the asset type from file path
    /// - Parameter path: File path
    static AssetType GetAssetTypeFromPath(const std::filesystem::path& path);
    /// This function returns the asset type from file Extesion
    /// - Parameter extension: Extesion
    static AssetType GetAssetTypeFromExtension(const std::string& extension);
    
    /// This function returns the asset meta data from handle
    static const AssetMetadata& GetMetadata(AssetHandle handle);
    /// This function returns the asset meta data from file path
    /// - Parameter filepath: Asset file path
    static const AssetMetadata& GetMetadata(const std::filesystem::path& filepath);
    
    /// This function checks the asset is memory asset
    /// - Parameter handle: Asset handle
    static bool IsMemoryAsset(AssetHandle handle);
    
    /// This functuion checks is asset handle is valid
    /// - Parameter assetHandle: Asset handle
    static bool IsAssetHandleValid(AssetHandle assetHandle);
    
    /// This function returns the asset registry
    static const AssetRegistry& GetAssetRegistry();
    
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
    
    /// This function imports the assets as store in registry
    /// - Parameter filepath: Asset file path
    static AssetHandle ImportAsset(const std::filesystem::path& filepath);
    
    // Template APIs ------------------------------------------------------------------------------------------------
    /// This function creates a memory asset that need to be stored`
    /// - Parameters:
    ///   - args: Arguments for asset creation
    ///   - fileName: file path of asset
    /// - Note :
    ///   TAsset - Type of asset
    ///   TArgs  - Dynamic arguments for asset creation
    template<typename TAsset, typename... TArgs>
    static AssetHandle CreateMemoryOnlyAsset(const std::string& fileName, TArgs&&... args)
    {
      static_assert(std::is_base_of<Asset, TAsset>::value,
                    "CreateMemoryOnlyAsset only works for types derived from Asset");
      
      Ref<TAsset> asset = TAsset::Create(std::forward<TArgs>(args)...);
      
      const auto& relPath = GetRelativePath(fileName);
      asset->handle = Hash::GenerateFNV(relPath);
      
      AssetMetadata metadata;
      metadata.handle = asset->handle;
      metadata.filePath = relPath;
      metadata.isDataLoaded = true;
      metadata.type = TAsset::GetStaticType();
      metadata.isMemoryAsset = true;
      
      s_assetRegistry[metadata.filePath] = metadata;
      s_memoryAssets[asset->handle] = asset;
      return asset->handle;
    }
    
    /// This function returns the asset from Asset handle
    /// - Parameter assetHandle: Asset Handle
    template<typename T> static Ref<T> GetAsset(AssetHandle assetHandle)
    {
      if (IsMemoryAsset(assetHandle))
      {
        return std::dynamic_pointer_cast<T>(s_memoryAssets[assetHandle]);
      }
      
      auto& metadata = GetMetadataInternal(assetHandle);
      if (!metadata.IsValid())
      {
        return nullptr;
      }
      
      Ref<Asset> asset = nullptr;
      if (!metadata.isDataLoaded)
      {
        metadata.isDataLoaded = AssetImporter::TryLoadData(metadata, asset);
        if (!metadata.isDataLoaded)
        {
          return nullptr;
        }
        s_loadedAssets[assetHandle] = asset;
      }
      else
      {
        asset = s_loadedAssets[assetHandle];
      }
      
      return std::dynamic_pointer_cast<T>(asset);
    }
    
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
    
    /// This function returns the metada from handle
    /// - Parameter handle: Asset handle
    static AssetMetadata& GetMetadataInternal(AssetHandle handle);
    
    // Member Variables ---------------------------------------------------------------------------------------------
    static AssetRegistry s_assetRegistry;
    static std::unordered_map<AssetHandle, Ref<Asset>> s_memoryAssets;
    static std::unordered_map<AssetHandle, Ref<Asset>> s_loadedAssets;
  };
} // namespace IKan
