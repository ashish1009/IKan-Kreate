//
//  AssetRegistry.hpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#pragma once

#include "Assets/AssetMetadata.hpp"

namespace IKan
{
  /// This class stores the assets in registry for thier paths
  class AssetRegistry
  {
  public:
    /// This operator retryrn the asset meta data from path
    /// - Parameter path: asset file path
    AssetMetadata& operator[](const std::filesystem::path& path);
    /// This function returns the Asset meta from path
    /// - Parameter path: Asset file path
    const AssetMetadata& Get(const std::filesystem::path& path) const;
    
    /// This function returns the count of asset registry
    size_t Count() const;
    /// This function checks is asset registry have this path
    /// - Parameter path: Asset file path
    bool Contains(const std::filesystem::path& path) const;
    /// This function rremoves the asset from registry
    /// - Parameter path: Asset file path
    size_t Remove(const std::filesystem::path& handle);
    /// This function clears the registry
    void Clear();
    
    // Iterators ----------------------------------------------------------------------------------------------------
    std::unordered_map<std::filesystem::path, AssetMetadata>::iterator begin() { return m_assetRegistry.begin(); }
    std::unordered_map<std::filesystem::path, AssetMetadata>::iterator end() { return m_assetRegistry.end(); }
    std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator begin() const { return m_assetRegistry.cbegin(); }
    std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator end() const { return m_assetRegistry.cend(); }
    std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator cbegin() const { return m_assetRegistry.cbegin(); }
    std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator cend() const { return m_assetRegistry.cend(); }
    
  private:
    std::unordered_map<std::filesystem::path, AssetMetadata> m_assetRegistry;
  };
} // namespace IKan
