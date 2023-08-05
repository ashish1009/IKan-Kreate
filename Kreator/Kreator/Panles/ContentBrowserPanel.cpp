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
  
  void ContentBrowserPanel::OnImGuiRender(bool &isOpen)
  {
    ImGui::Begin("Content Browser", &isOpen, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    {
      m_isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
      m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
      
      UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
      UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
      UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 2.0f));
      UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 15);
      
      ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
      UI::PushID();
      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        ImGui::TableSetupColumn("Outliner", 0, 300.0f);
        ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();
        
        // Content Outliner
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##folders_common");
        {
          UI::ScopedColor header(ImGuiCol_Header, UI::Theme::Color::PropertyField);
          if (ImGui::CollapsingHeader("Content", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
          {
            UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            UI::ScopedColorStack itemBg(ImGuiCol_Header, IM_COL32_DISABLE, ImGuiCol_HeaderActive, IM_COL32_DISABLE);

            if (m_baseDirectory)
            {
              std::vector<Ref<DirectoryInfo>> directories;
              directories.reserve(m_baseDirectory->subDirectories.size());
              for (auto& [handle, directory] : m_baseDirectory->subDirectories)
              {
                directories.emplace_back(directory);
              }
              
              std::sort(directories.begin(), directories.end(), [](const auto& a, const auto& b)
                        {
                return a->filePath.stem().string() < b->filePath.stem().string();
              });
              
              for (auto& directory : directories)
              {
                RenderDirectoryHierarchy(directory);
              }
            }
          }
        }
        
        // Draw side shadow
        ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 10.0f);
        ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
        UI::DrawShadowInner(m_shadowTexture, 15.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f,
                            false, true, false, false);
        ImGui::PopClipRect();
        ImGui::EndChild(); // folders_common
        
        // Directory Content
        ImGui::TableSetColumnIndex(1);
        const float topBarHeight = 26.0f;
        const float bottomBarHeight = 0.0f;
        ImGui::BeginChild("##directory_structure",
                          ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
        {
          ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
          RenderTopBar(topBarHeight);
          ImGui::PopStyleVar();
        }
        
        ImGui::EndChild(); // directory_structure
        ImGui::EndTable();
      }
      UI::PopID();
    }
    ImGui::End();
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
    std::string name = directory->filePath.filename().string();
    std::string id = name + "_TreeNode";
    bool previousState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(id.c_str()));

    // ImGui item height hack
    auto* window = ImGui::GetCurrentWindow();
    window->DC.CurrLineSize.y = 20.0f;
    window->DC.CurrLineTextBaseOffset = 3.0f;

    // Window Item Rectangle area
    const ImRect itemRect =
    {
      window->WorkRect.Min.x, window->DC.CursorPos.y,
      window->WorkRect.Max.x, window->DC.CursorPos.y + window->DC.CurrLineSize.y
    };

    // Lamda to check is item clicked
    const bool isItemClicked = [&itemRect, &id]
    {
      if (ImGui::ItemHoverable(itemRect, ImGui::GetID(id.c_str())))
      {
        return ImGui::IsMouseDown(ImGuiMouseButton_Left) or ImGui::IsMouseReleased(ImGuiMouseButton_Left);
      }
      return false;
    }();

    const bool isWindowFocused = ImGui::IsWindowFocused();

    // Fill with light selection colour if any of the child entities selected
    auto checkIfAnyDescendantSelected = [&](Ref<DirectoryInfo>& directory, auto isAnyDescendantSelected) -> bool
    {
      if (directory->handle == m_currentDirectory->handle)
      {
        return true;
      }
      
      if (!directory->subDirectories.empty())
      {
        for (auto& [childHandle, childDir] : directory->subDirectories)
        {
          if (isAnyDescendantSelected(childDir, isAnyDescendantSelected))
          {
            return true;
          }
        }
      }
      
      return false;
    };

    // Lamda to fill the item with color
    auto fillWithColour = [&](const ImColor& colour)
    {
      const ImU32 bgColour = ImGui::ColorConvertFloat4ToU32(colour);
      ImGui::GetWindowDrawList()->AddRectFilled(itemRect.Min, itemRect.Max, bgColour);
    };

    const bool isAnyDescendantSelected = checkIfAnyDescendantSelected(directory, checkIfAnyDescendantSelected);
    const bool isActiveDirectory = directory->handle == m_currentDirectory->handle;

    ImGuiTreeNodeFlags flags = (isActiveDirectory ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_SpanFullWidth;

    // Fill background
    //----------------
    if (isActiveDirectory or isItemClicked)
    {
      if (isWindowFocused)
      {
        fillWithColour(Kreator_UI::Color::Selection);
      }
      else
      {
        const ImColor col = UI::ColorWithMultipliedValue(Kreator_UI::Color::Selection, 0.8f);
        fillWithColour(UI::ColorWithMultipliedSaturation(col, 0.7f));
      }
      
      ImGui::PushStyleColor(ImGuiCol_Text, UI::Theme::Color::BackgroundDark);
    }
    else if (isAnyDescendantSelected)
    {
      fillWithColour(Kreator_UI::Color::SelectionMuted);
    }

    // Tree Node
    //----------
    bool open = UI::TreeNode(id, name, flags, m_folderIcon);
    
    if (isActiveDirectory or isItemClicked)
    {
      ImGui::PopStyleColor();
    }
    
    // Fixing slight overlap
    UI::ShiftCursorY(3.0f);

    // Draw children
    //--------------
    if (open)
    {
      std::vector<Ref<DirectoryInfo>> directories;
      directories.reserve(m_baseDirectory->subDirectories.size());
      for (auto& [handle, directory] : directory->subDirectories)
      {
        directories.emplace_back(directory);
      }
      
      std::sort(directories.begin(), directories.end(), [](const auto& a, const auto& b)
                {
        return a->filePath.stem().string() < b->filePath.stem().string();
      });
      
      for (auto& child : directories)
      {
        RenderDirectoryHierarchy(child);
      }
    }

    // Drop the Favourit folders
    UpdateDropArea(directory);

    if (open != previousState and !isActiveDirectory)
    {
      if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left, 0.01f))
      {
        ChangeDirectory(directory);
      }
    }

    // Close Popup
    if (open)
    {
      ImGui::TreePop();
    }
  }
  
  void ContentBrowserPanel::RenderTopBar(float height)
  {
    ImGui::BeginChild("##top_bar", ImVec2(0, height));
    ImGui::BeginHorizontal("##top_bar", ImGui::GetWindowSize());
    {
      const float edgeOffset = 4.0f;
      // Navigation buttons
      {
        UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 0.0f));
        
        // Render Icon Buttons
        auto contenBrowserButton = [height](const char* labelId, const Ref<Image>& icon)
        {
          const ImU32 buttonCol = UI::Theme::Color::BackgroundDark;
          const ImU32 buttonColP = UI::ColorWithMultipliedValue(UI::Theme::Color::BackgroundDark, 0.8f);
          UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonCol,
                                            ImGuiCol_ButtonHovered, buttonCol,
                                            ImGuiCol_ButtonActive, buttonColP);
          
          const float iconSize = std::min(24.0f, height);
          const float iconPadding = 3.0f;
          const bool clicked = ImGui::Button(labelId, ImVec2(iconSize, iconSize));
          UI::DrawButtonImage(icon, Kreator_UI::Color::TextDarker,
                              UI::ColorWithMultipliedValue(Kreator_UI::Color::TextDarker, 1.2f),
                              UI::ColorWithMultipliedValue(Kreator_UI::Color::TextDarker, 0.8f),
                              UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));
          
          return clicked;
        };
        
        if (contenBrowserButton("##back", m_backButton))
        {
          OnBrowseBack();
        }
        UI::SetTooltip("Previous directory");
        
        ImGui::Spring(-1.0f, edgeOffset);
        
        if (contenBrowserButton("##forward", m_forwardButton))
        {
          OnBrowseForward();
        }
        UI::SetTooltip("Next directory");
        
        ImGui::Spring(-1.0f, edgeOffset * 2.0f);
        
        if (contenBrowserButton("##refresh", m_refreshIcon))
        {
          Refresh();
        }
        UI::SetTooltip("Refresh");
        
        ImGui::Spring(-1.0f, edgeOffset * 2.0f);
      }
    }
    ImGui::EndHorizontal();
    ImGui::EndChild();
  }
  
  void ContentBrowserPanel::OnEvent(Event &e)
  {

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
        // If directory have some file that is not registered
        AssetType type = AssetManager::GetAssetTypeFromPath(entry.path());
        if (type == AssetType::None)
        {
          continue;
        }
        
        // If that file is valid for Kreator then Import and push in registry
        metadata.handle = AssetManager::ImportAsset(entry.path());
      }
      
      // Failed to import
      if (!metadata.IsValid())
      {
        // TODO: Temp assert to check code hit
        IK_ASSERT(false);
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
    if (target->handle != m_currentDirectory->handle and ImGui::BeginDragDropTarget())
    {
      // TODO: Implement  
      IK_ASSERT(false);
    }
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
