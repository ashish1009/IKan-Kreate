//
//  ContentBrowserPanel.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "ContentBrowserItem.hpp"

namespace Kreator
{
  /// This class stores the selection Items in a stack
  class SelectionStack : public VectorIterator<AssetHandle>
  {
  public:
    /// Default constructor
    SelectionStack();
    
    /// This fucntion copy the selection
    /// - Parameter other: other selection stack
    void CopyFrom(const SelectionStack& other);
    /// This function select a item
    /// - Parameter handle: item handle
    void Select(AssetHandle handle);
    /// This function deselect a item
    /// - Parameter handle: item handle
    void Deselect(AssetHandle handle);
    /// This function clears the item selection
    void Clear();
    /// This function checks if item is selectd
    /// - Parameter handle: item handle
    bool IsSelected(AssetHandle handle) const;
    /// This function returns the selection counts
    size_t SelectionCount() const;
    /// This function returns the selection data
    const AssetHandle* SelectionData() const;
    /// This function returns the item at index
    AssetHandle operator[](size_t index) const;
    
  private:
    std::vector<AssetHandle> m_selections;
  };
  
  /// This structure stores the content browser panel item in list
  struct ContentBrowserItemList : public VectorIterator<Ref<ContentBrowserItem>>
  {
    static constexpr size_t InvalidItem = std::numeric_limits<size_t>::max();
    std::vector<Ref<ContentBrowserItem>> items;
    
    /// Default constructor
    ContentBrowserItemList();
    
    // Operators ----------------------------------------------------------------------------------------------------
    Ref<ContentBrowserItem>& operator[](size_t index) { return items[index]; }
    const Ref<ContentBrowserItem>& operator[](size_t index) const { return items[index]; }
    
    /// This function clears the items
    void Clear();
    /// This function erases the item
    /// - Parameter handle: Item Handle to be removed
    void Erase(AssetHandle handle);
    /// This function searches the item from list
    /// - Parameter handle: Item Handle
    size_t FindItem(AssetHandle handle) const;
    
    /// Copy operator
    /// - Parameter other: other
    ContentBrowserItemList& operator=(const ContentBrowserItemList& other);
  };

  /// This class creates the content browser panel
  class ContentBrowserPanel : public EditorPanel
  {
  public:
    /// Default Constructor
    ContentBrowserPanel();
    
    /// @see Editor Panel for Documentation
    void OnImGuiRender(bool& isOpen) override;
    /// @see Editor Panel for Documentation
    void OnEvent(Event& e) override;
    /// @see Editor Panel for Documentation
    void OnProjectChanged(const Ref<Project>& project) override;
    
    // Getters -----------------------------------------------------------------------------------------------------
    /// This function returns the directory from the map
    /// - Parameter filepath Directory file path
    Ref<DirectoryInfo> GetDirectory(const std::filesystem::path& filepath) const;
    /// This function returns the Current Items
    ContentBrowserItemList& GetCurrentItems();
    /// This function returns the selection stack
    const SelectionStack& GetSelectionStack() const;
    /// This function returns the static singleton Instance
    static ContentBrowserPanel& Get();
    
  private:
    // Member Functions ---------------------------------------------------------------------------------------------
    /// This function Process the direcotry recursivly
    /// - Parameters:
    ///   - directoryPath: Directory path
    ///   - parent: Directory parent path
    AssetHandle ProcessDirectory(const std::filesystem::path& directoryPath, const Ref<DirectoryInfo>& parent);
    /// This function changes the directory
    /// - Parameter directory: New Directory
    void ChangeDirectory(Ref<DirectoryInfo>& directory);
    /// This function sorts the item list in CBP
    void SortItemList();
    /// This function clears the selection of Items
    void ClearSelections();
    /// This function renders the directory hierarchy
    /// - Parameter directory: Directory
    void RenderDirectoryHierarchy(Ref<DirectoryInfo>& directory);
    /// This function updatres the ImGui Drop content
    /// - Parameter target: Target Directory
    void UpdateDropArea(const Ref<DirectoryInfo>& target);
    
    /// This function renders the Top bar of content browser panel
    /// - Parameter height Height of top bar
    void RenderTopBar(float height);
    /// This function renders the item in Main Area
    void RenderItems();
    /// This function renders the delete Dialog box
    void RenderDeleteDialogue();
    
    /// This function searches the items from Lists
    /// - Parameters:
    ///   - query: Search query
    ///   - directoryInfo: directory infor
    ContentBrowserItemList Search(const std::string& query, const Ref<DirectoryInfo>& directoryInfo);
    /// This function Pasets the Copied Data
    void PasteCopiedAssets();
    /// This function Updates the keyboard Inputs
    void UpdateInput();
    /// This function removes the direcorty from list
    /// - Parameter directory: direcotries
    void RemoveDirectory(Ref<DirectoryInfo>& directory, bool removeFromParent = true);
    
    // Navigations ------------------------
    /// This function Uopdate the Back button function
    void OnBrowseBack();
    /// This function Uopdate the Forward button function
    void OnBrowseForward();
    /// This function Uopdate the Refresh button function
    void Refresh();
    
    // NOTE: This should only be used within the ContentBrowserPanel!
    //     For creating a new asset outside the content browser, use AssetManager::CreateNewAsset!
    template<typename T, typename... Args>
    Ref<T> CreateAsset(const std::string& filename, Args&&... args)
    {
      Ref<T> asset = AssetManager::CreateNewAsset<T>(filename, m_currentDirectory->filePath.string(), std::forward<Args>(args)...);
      if (!asset)
      {
        return nullptr;
      }
      
      m_currentDirectory->assets.push_back(asset->handle);
      ChangeDirectory(m_currentDirectory);
      
      auto& item = m_currentItems[m_currentItems.FindItem(asset->handle)];
      m_selectionStack.Select(asset->handle);
      item->SetSelected(true);
      item->StartRenaming();
      
      return asset;
    }
    
    // Member Variables ---------------------------------------------------------------------------------------------
    bool m_isHovered;
    bool m_isFocused;
    bool m_isAnyItemHovered = false;
    bool m_updateNavigationPath = false;
    bool m_showAssetType = true;
    
    Ref<Project> m_project;
    
    Ref<Image> m_fileTex;
    Ref<Image> m_folderIcon;
    Ref<Image> m_shadowTexture;
    Ref<Image> m_backButton, m_forwardButton, m_refreshIcon;
    std::map<std::string, Ref<Image>> m_assetIconMap;
    
    std::unordered_map<AssetHandle, Ref<DirectoryInfo>> m_directories;
    std::vector<Ref<DirectoryInfo>> m_breadCrumbData;
    ContentBrowserItemList m_currentItems;
    Ref<DirectoryInfo> m_currentDirectory;
    Ref<DirectoryInfo> m_baseDirectory;
    Ref<DirectoryInfo> m_nextDirectory, m_previousDirectory;
    
    SelectionStack m_selectionStack, m_copiedAssets;
    
    char m_searchBuffer[MAX_INPUT_BUFFER_LENGTH];
    
    inline static ContentBrowserPanel* s_instance;
  };
} // namespace Kreator
