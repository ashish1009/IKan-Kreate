//
//  AssetRegistry.hpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#pragma once

#include "Asset/AssetMetadata.hpp"

namespace IKan
{
  /// This class Registers the Asset. Stores all the asset in a map of file path
  class AssetRegistry
  {
  public:
    /// This operator retryrn the asset meta data from path
    /// - Parameter path: asset file path
    AssetMetadata& operator[](const AssetHandle& handle);
    /// This function returns the Asset meta from path
    /// - Parameter path: Asset file path
    const AssetMetadata& Get(const AssetHandle& handle) const;
    
    /// This function returns the count of asset registry
    size_t Count() const;
    /// This function checks is asset registry have this path
    /// - Parameter path: Asset file path
    bool Contains(const AssetHandle& handle) const;
    /// This function checks is asset registry have this path
    /// - Parameter path: Asset file path
    bool Contains(const std::filesystem::path& path) const;
    /// This function rremoves the asset from registry
    /// - Parameter path: Asset file path
    size_t Remove(const AssetHandle& handle);
    /// This function checks is asset registry have this path
    /// - Parameter path: Asset file path
    const AssetMetadata& GetMetadata(const std::filesystem::path& path) const;
    /// This function clears the registry
    void Clear();
    
    // Iterators ----------------------------------------------------------------------------------------------------
    std::unordered_map<AssetHandle, AssetMetadata>::iterator begin() { return m_assetRegistry.begin(); }
    std::unordered_map<AssetHandle, AssetMetadata>::iterator end() { return m_assetRegistry.end(); }
    std::unordered_map<AssetHandle, AssetMetadata>::const_iterator begin() const { return m_assetRegistry.cbegin(); }
    std::unordered_map<AssetHandle, AssetMetadata>::const_iterator end() const { return m_assetRegistry.cend(); }
    std::unordered_map<AssetHandle, AssetMetadata>::const_iterator cbegin() const { return m_assetRegistry.cbegin(); }
    std::unordered_map<AssetHandle, AssetMetadata>::const_iterator cend() const { return m_assetRegistry.cend(); }
    
  private:
    std::unordered_map<AssetHandle, AssetMetadata> m_assetRegistry;
  };
} // namespace IKan
