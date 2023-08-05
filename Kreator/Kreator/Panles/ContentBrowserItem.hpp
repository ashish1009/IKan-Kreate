//
//  ContentBrowserItem.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
  /// This structure stores the Directory Info
  struct DirectoryInfo
  {
    AssetHandle handle;
    Ref<DirectoryInfo> parent;
    std::filesystem::path filePath;
    std::vector<AssetHandle> assets;
    std::map<AssetHandle, Ref<DirectoryInfo>> subDirectories;
  };
} // namespace Kreator
