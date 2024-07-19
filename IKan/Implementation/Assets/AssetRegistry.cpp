//
//  AssetRegistry.cpp
//  IKan
//
//  Created by Ashish . on 19/07/24.
//

#include "AssetRegistry.hpp"

namespace IKan
{
  AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& path)
  {
    IK_ASSERT(!path.string().empty());
    return m_assetRegistry[path];
  }
  
  const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path) const
  {
    IK_ASSERT(m_assetRegistry.find(path) != m_assetRegistry.end());
    return m_assetRegistry.at(path);
  }
  
  bool AssetRegistry::Contains(const std::filesystem::path& path) const
  {
    return m_assetRegistry.find(path) != m_assetRegistry.end();
  }
  
  size_t AssetRegistry::Remove(const std::filesystem::path& path)
  {
    return m_assetRegistry.erase(path);
  }
  
  void AssetRegistry::Clear()
  {
    IK_LOG_TRACE(LogModule::Asset, "Clearing Asset registry");
    m_assetRegistry.clear();
  }
  
  size_t AssetRegistry::Count() const
  {
    return m_assetRegistry.size();
  }
} // namespace IKan
