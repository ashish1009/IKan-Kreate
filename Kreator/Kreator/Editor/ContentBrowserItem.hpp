//
//  ContentBrowserItem.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator
{
#define MAX_INPUT_BUFFER_LENGTH 128
  
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
