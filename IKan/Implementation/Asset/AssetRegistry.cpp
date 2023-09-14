//
//  AssetRegistry.cpp
//  IKan
//
//  Created by Ashish . on 30/07/23.
//

#include "AssetRegistry.hpp"
#include "Project/Project.hpp"

namespace IKan
{
#define IK_ASSETREGISTRY_LOG 0
#if IK_ASSETREGISTRY_LOG
#define ASSET_LOG(...) IK_LOG_TRACE(__VA_ARGS__)
#else
#define ASSET_LOG(...)
#endif
    
  AssetMetadata& AssetRegistry::operator[](const AssetHandle& handle)
  {
    return m_assetRegistry[handle];
  }

  const AssetMetadata& AssetRegistry::GetMetadata(const std::filesystem::path &path) const
  {
    for (const auto& [handle, metadata] : m_assetRegistry)
    {
      if (metadata.filePath == path)
      {
        return metadata;
      }
    }
    IK_ASSERT(false);
  }

  const AssetMetadata& AssetRegistry::Get(const AssetHandle& handle) const
  {
    return m_assetRegistry.at(handle);
  }
  
  bool AssetRegistry::Contains(const AssetHandle& handle) const
  {
    return m_assetRegistry.find(handle) != m_assetRegistry.end();
  }

  bool AssetRegistry::Contains(const std::filesystem::path& path) const
  {
    for (const auto& [handle, metadata] : m_assetRegistry)
    {
      if (metadata.filePath == path)
      {
        return true;
      }
    }
    return false;
  }

  size_t AssetRegistry::Remove(const AssetHandle& handle)
  {
    return m_assetRegistry.erase(handle);
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
