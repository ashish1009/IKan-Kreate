//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserPanel.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  static float s_padding = 2.0f;

  // Selection Stack ------------------------------------------------------------------------------------------------
  void SelectionStack::CopyFrom(const SelectionStack& other)
  {
    m_selections.assign(other.begin(), other.end());
  }
  
  void SelectionStack::Select(AssetHandle handle)
  {
    if (IsSelected(handle))
    {
      return;
    }
    
    m_selections.push_back(handle);
  }
  
  void SelectionStack::Deselect(AssetHandle handle)
  {
    if (!IsSelected(handle))
    {
      return;
    }
    
    for (auto it = m_selections.begin(); it != m_selections.end(); it++)
    {
      if (handle == *it)
      {
        m_selections.erase(it);
        break;
      }
    }
  }
  
  bool SelectionStack::IsSelected(AssetHandle handle) const
  {
    for (const auto& selectedHandle : m_selections)
    {
      if (selectedHandle == handle)
      {
        return true;
      }
    }
    
    return false;
  }
  
  void SelectionStack::Clear()
  {
    m_selections.clear();
  }
  
  size_t SelectionStack::SelectionCount() const
  {
    return m_selections.size();
  }
  const AssetHandle* SelectionStack::SelectionData() const
  {
    return m_selections.data();
  }
  
  AssetHandle SelectionStack::operator[](size_t index) const
  {
    IK_ASSERT(index >= 0 && index < m_selections.size());
    return m_selections[index];
  }
  
  // Content Browser Item List -------------------------------------------------------------------------------------------
  void ContentBrowserItemList::Clear()
  {
    items.clear();
  }
  void ContentBrowserItemList::Erase(AssetHandle handle)
  {
    size_t index = FindItem(handle);
    if (index == InvalidItem)
    {
      return;
    }
    
    auto it = items.begin() + index;
    items.erase(it);
  }
  
  size_t ContentBrowserItemList::FindItem(AssetHandle handle) const
  {
    for (size_t i = 0; i < items.size(); i++)
    {
      if (items[i]->GetID() == handle)
      {
        return i;
      }
    }
    return InvalidItem;
  }
  
  // Content Browser Panel -------------------------------------------------------------------------------------------
  namespace CBP_Utils
  {
    /// Get Full Content browser panel path
    /// - Parameter file_name: file name
    Ref<Image> AssetPath(const std::string& fileName)
    {
      return Image::Create(RendererLayer::GetClientResorucePath() / "Textures/CBP" / fileName);
    }
  } // namespace CBP_Utils
  
  ContentBrowserPanel::ContentBrowserPanel()
  {
    IK_LOG_TRACE("ContentBrowserPanel", "Initialising Content Browser Panel Textures");
    s_instance = this;
    
    m_shadowTexture = CBP_Utils::AssetPath("Shadow.png");
    
    m_fileTex = CBP_Utils::AssetPath("File.png");
    m_folderIcon = CBP_Utils::AssetPath("Folder.png");
    
    m_backButton = CBP_Utils::AssetPath("Back.png");
    m_forwardButton = CBP_Utils::AssetPath("Forward.png");
    m_refreshIcon = CBP_Utils::AssetPath("Refresh.png");
    
    m_assetIconMap[".png"] = CBP_Utils::AssetPath("Png.png");
    m_assetIconMap[".ttf"] = CBP_Utils::AssetPath("Font.png");
    m_assetIconMap[".ttc"] = m_assetIconMap.at(".ttf");
    m_assetIconMap[".otf"] = m_assetIconMap.at(".ttf");
    
    m_assetIconMap[".ikscene"] = CBP_Utils::AssetPath("IKScene.png");
  }
  
  void ContentBrowserPanel::OnImguiRender(bool &isOpen)
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::RenderDeleteDialogue()
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::RemoveDirectory(Ref<DirectoryInfo>& directory, bool removeFromParent)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::RenderItems()
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::RenderDirectoryHierarchy(Ref<DirectoryInfo> &directory)
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::RenderTopBar(float height)
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::OnEvent(Event &e)
  {
    IK_ASSERT(false);

  }
  
  void ContentBrowserPanel::OnProjectChanged(const Ref<Project> &project)
  {
    if (!project)
    {
      return;
    }
    
    // Clear all the data if project is changed
    m_directories.clear();
    m_currentItems.Clear();
    m_baseDirectory = nullptr;
    m_currentDirectory = nullptr;
    m_nextDirectory = nullptr;
    m_previousDirectory = nullptr;
    m_selectionStack.Clear();
    m_breadCrumbData.clear();
    
    // Updater the project
    m_project = project;
    
    AssetHandle baseDirectoryHandle = ProcessDirectory(project->GetAssetDirectory().string(), nullptr);
    m_baseDirectory = m_directories[baseDirectoryHandle];
    ChangeDirectory(m_baseDirectory);
  }
  
  AssetHandle ContentBrowserPanel::ProcessDirectory(const std::filesystem::path &directoryPath, const Ref<DirectoryInfo> &parent)
  {
    // Get Directory from pre processed directories
    const auto& directory = GetDirectory(directoryPath);
    
    // If directory already preprocessed then do not process again
    if (directory)
    {
      return directory->handle;
    }
    
    // Create directory info instance for storing in directories
    Ref<DirectoryInfo> directoryInfo = CreateRef<DirectoryInfo>();
    directoryInfo->handle = AssetHandle();
    directoryInfo->parent = parent;
    
    if (directoryPath == m_project->GetAssetDirectory())
    {
      directoryInfo->filePath = "";
    }
    else
    {
      directoryInfo->filePath = std::filesystem::relative(directoryPath, m_project->GetAssetDirectory());
    }
    
    // Add Subdirectories or assets recursivally
    for (auto entry : std::filesystem::directory_iterator(directoryPath))
    {
      if (entry.is_directory())
      {
        AssetHandle subdirHandle = ProcessDirectory(entry.path(), directoryInfo);
        directoryInfo->subDirectories[subdirHandle] = m_directories[subdirHandle];
        continue;
      }
      
      auto metadata = AssetManager::GetMetadata(std::filesystem::relative(entry.path(), m_project->GetAssetDirectory()));
      if (!metadata.IsValid())
      {
        AssetType type = AssetManager::GetAssetTypeFromPath(entry.path());
        if (type == AssetType::None)
        {
          continue;
        }
        metadata.handle = AssetManager::ImportAsset(entry.path());
      }
      
      // Failed to import
      if (!metadata.IsValid())
      {
        continue;
      }
      
      directoryInfo->assets.push_back(metadata.handle);
    }
    
    // Store the directory info in memory to reuse them
    m_directories[directoryInfo->handle] = directoryInfo;
    return directoryInfo->handle;
  }
  
  Ref<DirectoryInfo> ContentBrowserPanel::GetDirectory(const std::filesystem::path& filepath) const
  {
    if (filepath.string() == "" || filepath.string() == ".")
    {
      return m_baseDirectory;
    }
    
    for (const auto&[handle, directory] : m_directories)
    {
      if (directory->filePath == filepath)
      {
        return directory;
      }
    }
    return nullptr;
  }
  
  void ContentBrowserPanel::ChangeDirectory(Ref<DirectoryInfo> &directory)
  {
    // Check if directory is valid
    if (!directory)
    {
      return;
    }
    
    m_updateNavigationPath = true;
    
    // Clear the current item list and update with new subdirectories
    m_currentItems.items.clear();
    for (auto&[subdirHandle, subdir] : directory->subDirectories)
    {
      // Create Item list for directory
      m_currentItems.items.push_back(CreateRef<ContentBrowserDirectory>(subdir, m_folderIcon));
    }
    
    // Update the Assets in Current item lists
    for (auto assetHandle : directory->assets)
    {
      AssetMetadata metadata = AssetManager::GetMetadata(assetHandle);
      if (metadata.IsValid())
      {
        // Set the asset texture
        Ref<Texture> assetTexture = m_fileTex;
        if (m_assetIconMap.find(metadata.filePath.extension().string()) != m_assetIconMap.end())
        {
          assetTexture = m_assetIconMap[metadata.filePath.extension().string()];
        }
        
        // Create Item list for asset
        m_currentItems.items.push_back(CreateRef<ContentBrowserAsset>(metadata, assetTexture));
      }
    }
    
    SortItemList();
    
    m_previousDirectory = directory;
    m_currentDirectory = directory;
    
    ClearSelections();
  }
  
  void ContentBrowserPanel::SortItemList()
  {
    std::sort(m_currentItems.begin(), m_currentItems.end(), [](const Ref<ContentBrowserItem>& item1, const Ref<ContentBrowserItem>& item2)
              {
      if (item1->GetType() == item2->GetType())
      {
        return Utils::String::ToLower(item1->GetName()) < Utils::String::ToLower(item2->GetName());
      }
      return (uint16_t)item1->GetType() < (uint16_t)item2->GetType();
    });
  }
  
  void ContentBrowserPanel::ClearSelections()
  {
    for (auto& item : m_currentItems)
    {
      item->SetSelected(false);
      
      if (item->IsRenaming())
      {
        item->StopRenaming();
      }
    }
    
    m_selectionStack.Clear();
  }
  
  void ContentBrowserPanel::UpdateDropArea(const Ref<DirectoryInfo>& target)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::OnBrowseBack()
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::OnBrowseForward()
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::Refresh()
  {
    IK_ASSERT(false);
  }
  
  ContentBrowserItemList ContentBrowserPanel::Search(const std::string &query, const Ref<DirectoryInfo> &directoryInfo)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::PasteCopiedAssets()
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::UpdateInput()
  {
    IK_ASSERT(false);
  }
  
  ContentBrowserItemList& ContentBrowserPanel::GetCurrentItems()
  {
    return m_currentItems;
  }
  
  const SelectionStack& ContentBrowserPanel::GetSelectionStack() const
  {
    return m_selectionStack;
  }
  
  ContentBrowserPanel& ContentBrowserPanel::Get()
  {
    return *s_instance;
  }

} // namespace IKan
