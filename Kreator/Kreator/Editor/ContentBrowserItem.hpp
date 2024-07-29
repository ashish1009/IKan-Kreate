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
  
  enum class ContentBrowserAction
  {
    None                = 0,
    Refresh             = BIT(0),
    ClearSelections     = BIT(1),
    Selected            = BIT(2),
    Hovered             = BIT(3),
    Renamed             = BIT(4),
    NavigateToThis      = BIT(5),
    OpenDeleteDialogue  = BIT(6),
    SelectToHere        = BIT(7),
    Moved               = BIT(8),
    ShowInExplorer      = BIT(9),
    OpenExternal        = BIT(10),
    Reload              = BIT(11),
    Copy                = BIT(12),
    Duplicate           = BIT(13),
    StartRenaming       = BIT(14)
  };
  
  /// This structure stores all the actions for an item in CBP
  struct CBItemActionResult
  {
    uint16_t Field = 0;
    
    /// This function set an action in CBI
    /// - Parameters:
    ///   - flag: Action flag
    ///   - value: Set reset value
    void Set(ContentBrowserAction flag, bool value);
    /// This function check if an item have an action
    /// - Parameter flag: action flag
    bool IsSet(ContentBrowserAction flag) const;
  };

} // namespace Kreator
