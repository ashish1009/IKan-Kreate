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

  /// This structure stores the Content browser items base class
  class ContentBrowserItem
  {
  public:
    /// This is the CB Item type
    enum class ItemType : uint16_t
    {
      Directory, Asset
    };
    
    /// This is the content browser Item Constructor
    /// - Parameters:
    ///   - type: Item Type
    ///   - id: Asset Handle ID
    ///   - name: Asset Name
    ///   - icon: Texture Icon
    ContentBrowserItem(ItemType type, AssetHandle id, const std::string& name, const Ref<Image>& icon);
    /// This is the content browser Item Destructor
    virtual ~ContentBrowserItem() = default;
    
    // Virtual APIs ------------------------------------------------------------------------------------------------
    /// This function deletes the Item
    virtual void Delete() {}
    /// This function moves the Item to new destinateion
    /// - Parameter destination: New destination
    virtual bool Move([[maybe_unused]] const std::filesystem::path& destination) { return false; }
    /// This function activate the Item
    /// - Parameter actionResult: Action
    virtual void Activate([[maybe_unused]] CBItemActionResult& actionResult) {}
    
    // Fundamentals ------------------------------------------------------------------------------------------------
    /// This function Begins the Gui Render
    void OnRenderBegin();
    /// This function Renders the Item
    CBItemActionResult OnRender();
    /// This function ends the Render Gui
    void OnRenderEnd();
    
    /// This function starts the rename of items
    void StartRenaming();
    /// This function stops the rename of items
    void StopRenaming();
    /// This function set the select item
    /// - Parameter value: New set flag
    void SetSelected(bool value);
    /// This function rename the item
    /// - Parameter newName: New name
    void Rename(const std::string& newName);
    
    // Getters ------------------------------------------------------------------------------------------------------
    /// This function Returns the ID of Item
    AssetHandle GetID() const;
    /// This function Returns the Type of Item
    ItemType GetType() const;
    /// This function Returns the Name of Item
    const std::string& GetName() const;
    /// This function Returns the Texture Icon of Item
    const Ref<Image>& GetIcon() const;
    /// This function Checks is item selected
    bool IsSelected() const;
    /// This function checks is item is renaming
    bool IsRenaming() const;
    
  private:
    /// This function Handle Item Rename
    /// - Parameter newName: New namme
    virtual void OnRenamed(const std::string& newName);
    /// This function Render custom items
    virtual void RenderCustomContextItems() {}
    /// This function handles the drop of item
    /// - Parameter actionResult: Action results
    virtual void UpdateDrop([[maybe_unused]] CBItemActionResult& actionResult) {}
    /// This function handles the context menu open
    /// - Parameter actionResult: Actions
    void OnContextMenuOpen(CBItemActionResult& actionResult);
    
  protected:
    ItemType m_type;
    AssetHandle m_ID;
    std::string m_name;
    Ref<Image> m_icon;
    
    bool m_isSelected = false;
    bool m_isRenaming = false;
    bool m_isDragging = false;
  };
  
  /// This structure stores the Content browser items (Directory)
  class ContentBrowserDirectory : public ContentBrowserItem
  {
  public:
    ContentBrowserDirectory(const Ref<DirectoryInfo>& directoryInfo, const Ref<Image>& icon);
    virtual ~ContentBrowserDirectory() = default;
    
    Ref<DirectoryInfo>& GetDirectoryInfo() { return m_directoryInfo; }
    
    /// @see ContentBrowsetItem
    virtual void Delete() override;
    /// @see ContentBrowsetItem
    virtual bool Move(const std::filesystem::path& destination) override;
    
  private:
    /// @see ContentBrowsetItem
    virtual void Activate(CBItemActionResult& actionResult) override;
    /// @see ContentBrowsetItem
    virtual void OnRenamed(const std::string& newName) override;
    /// @see ContentBrowsetItem
    virtual void UpdateDrop(CBItemActionResult& actionResult) override;
    
    /// This function updates the directory path
    /// - Parameters:
    ///   - directoryInfo: Directory Info
    ///   - newParentPath: New path
    ///   - newName: New name
    void UpdateDirectoryPath(Ref<DirectoryInfo> directoryInfo, const std::filesystem::path& newParentPath,
                             const std::filesystem::path& newName);
    
  private:
    Ref<DirectoryInfo> m_directoryInfo;
  };

  /// This structure stores the Content browser items (Assets)
  class ContentBrowserAsset : public ContentBrowserItem
  {
  public:
    ContentBrowserAsset(const AssetMetadata& assetInfo, const Ref<Image>& icon);
    virtual ~ContentBrowserAsset() = default;
    
    const AssetMetadata& GetAssetInfo() const { return m_assetInfo; }
    
    /// @see ContentBrowsetItem
    virtual void Delete() override;
    /// @see ContentBrowsetItem
    virtual bool Move(const std::filesystem::path& destination) override;
    
  private:
    /// @see ContentBrowsetItem
    virtual void Activate(CBItemActionResult& actionResult) override;
    /// @see ContentBrowsetItem
    virtual void OnRenamed(const std::string& newName) override;
    
  private:
    AssetMetadata m_assetInfo;
  };

} // namespace Kreator
