//
//  AssetRegistry.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetRegistry.hpp"

namespace IKan
{
#define IK_ASSETREGISTRY_LOG 0
#if IK_ASSETREGISTRY_LOG
#define ASSET_LOG(...) IK_LOG_TRACE(__VA_ARGS__)
#else
#define ASSET_LOG(...)
#endif
  
  std::filesystem::path AssetRegistry::GetPathKey(const std::filesystem::path& path) const
  {
    IK_ASSERT(false);
  }
  
  AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& path)
  {
    auto key = GetPathKey(path);
    ASSET_LOG("[ASSET] Retrieving key {0} (path = {1})", key.string(), path.string());
    IK_ASSERT(!path.string().empty());
    return m_assetRegistry[key];
  }
  
  const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path) const
  {
    auto key = GetPathKey(path);
    IK_ASSERT(m_assetRegistry.find(key) != m_assetRegistry.end());
    ASSET_LOG("[ASSET] Retrieving const {0} (path = {1})", key.string(), path.string());
    IK_ASSERT(!path.string().empty());
    return m_assetRegistry.at(key);
  }
  
  bool AssetRegistry::Contains(const std::filesystem::path& path) const
  {
    auto key = GetPathKey(path);
    ASSET_LOG("[ASSET] Contains key {0} (path = {1})", key.string(), path.string());
    return m_assetRegistry.find(key) != m_assetRegistry.end();
  }
  
  size_t AssetRegistry::Remove(const std::filesystem::path& path)
  {
    auto key = GetPathKey(path);
    ASSET_LOG("[ASSET] Removing key {0} (path = {1})", key.string(), path.string());
    return m_assetRegistry.erase(key);
  }
  
  void AssetRegistry::Clear()
  {
    ASSET_LOG("[ASSET] Clearing registry");
    m_assetRegistry.clear();
  }
  
  size_t AssetRegistry::Count() const
  {
    return m_assetRegistry.size();
  }
} // namespace IKan
