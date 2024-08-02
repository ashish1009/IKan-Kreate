//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ContentBrowserPanel.hpp"
#include "Layers/RendererLayer.hpp"
#include "Editor/ApplicationSettings.hpp"

namespace Kreator
{
  static float s_padding = 2.0f;
  
  // Selection Stack ------------------------------------------------------------------------------------------------
  SelectionStack::SelectionStack()
  : VectorIterator<AssetHandle>(m_selections)
  {
    
  }
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
    IK_ASSERT(index >= 0 and index < m_selections.size());
    return m_selections[index];
  }
  
  // Content Browser Item List -------------------------------------------------------------------------------------------
  ContentBrowserItemList::ContentBrowserItemList()
  : VectorIterator<Ref<ContentBrowserItem>>(items)
  {
    
  }
  
  ContentBrowserItemList& ContentBrowserItemList::operator=(const ContentBrowserItemList& other)
  {
    items = other.items;
    return *this;
  }
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
    
    auto it = items.begin() + (uint32_t)index;
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
    static Ref<Image> AssetPath(const std::string& fileName)
    {
      return TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/CBP" / fileName);
    }
  } // namespace CBP_Utils
  
  ContentBrowserPanel::ContentBrowserPanel()
  {
    IK_PROFILE();
    IK_LOG_TRACE("ContentBrowserPanel", "Initialising Content Browser Panel Textures");
    s_instance = this;
    
    m_shadowTexture = CBP_Utils::AssetPath("Shadow.png");
    
    m_fileTex = CBP_Utils::AssetPath("File.png");
    m_folderIcon = CBP_Utils::AssetPath("Folder.png");
    
    m_backButton = CBP_Utils::AssetPath("Back.png");
    m_forwardButton = CBP_Utils::AssetPath("Forward.png");
    m_refreshIcon = CBP_Utils::AssetPath("Refresh.png");
    
    m_assetIconMap[".png"] = CBP_Utils::AssetPath("Png.png");
    m_assetIconMap[".jpg"] = m_assetIconMap.at(".png");
    m_assetIconMap[".hdr"] = m_assetIconMap.at(".png");
    
    m_assetIconMap[".ttf"] = CBP_Utils::AssetPath("Font.png");
    m_assetIconMap[".ttc"] = m_assetIconMap.at(".ttf");
    m_assetIconMap[".otf"] = m_assetIconMap.at(".ttf");
    
    m_assetIconMap[SceneExtension] = CBP_Utils::AssetPath("IKScene.png");
    m_assetIconMap[MaterialExtension] = CBP_Utils::AssetPath("Material.png");
    
    m_assetIconMap[".fbx"] = CBP_Utils::AssetPath("Fbx.png");
    m_assetIconMap[".obj"] = CBP_Utils::AssetPath("Obj.png");
    
    m_assetIconMap[PrefabExtension] = CBP_Utils::AssetPath("PrefabIcon.png");
  }
  
  void ContentBrowserPanel::OnImGuiRender(bool &isOpen)
  {
    IK_PERFORMANCE("ContentBrowserPanel::OnImGuiRender");
    ImGui::Begin("Content Browser", &isOpen, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
    {
      m_isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
      m_isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
      
      UI::ScopedColor bgColor(ImGuiCol_ChildBg, UI::Color::Background);
      UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
      UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(4.0f, 4.0f));
      UI::ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 2.0f));
      UI::ScopedStyle rouning(ImGuiStyleVar_FrameRounding, 15);
      
      ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;
      UI::PushID();
      if (ImGui::BeginTable(UI::GenerateID(), 2 /* Num Columns */, tableFlags, ImVec2(0.0f, 0.0f)))
      {
        ImGui::TableSetupColumn("Outliner", 0, 200.0f);
        ImGui::TableSetupColumn("Directory Structure", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableNextRow();
        
        // Content Outliner
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##folders_common");
        {
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
        UI::DrawShadowInner(m_shadowTexture, UI::Position::Right, 15.0f, windowRect, 1.0f, windowRect.GetHeight() / 4.0f);
        ImGui::PopClipRect();
        ImGui::EndChild(); // folders_common
        
        // Directory Content
        ImGui::TableSetColumnIndex(1);
        const float topBarHeight = 26.0f;
        const float bottomBarHeight = 0.0f;
        ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - topBarHeight - bottomBarHeight));
        {
          ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
          RenderTopBar(topBarHeight);
          ImGui::PopStyleVar();
          ImGui::Separator();
          
          ImGui::BeginChild("Main Area");
          {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.35f));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
            
            // Right click Menu --------------------------------------------------------------------------------------
            if (ImGui::BeginPopupContextWindow(0))
            {
              if (ImGui::BeginMenu("New"))
              {
                if (ImGui::MenuItem("Folder"))
                {
                  bool created = std::filesystem::create_directory(Project::GetAssetDirectory() / m_currentDirectory->filePath / "New Folder");
                  if (created)
                  {
                    Refresh();
                    const auto& directoryInfo = GetDirectory(m_currentDirectory->filePath / "New Folder");
                    size_t index = m_currentItems.FindItem(directoryInfo->handle);
                    
                    if (index != ContentBrowserItemList::InvalidItem)
                    {
                      m_currentItems[index]->StartRenaming();
                    }
                  }
                }
                
                ImGui::EndMenu();
              }
              
              ImGui::Separator();
              if (ImGui::MenuItem("Refresh"))
              {
                Refresh();
              }
              
              ImGui::Separator();
              if (ImGui::MenuItem("Copy", "Ctrl+C", nullptr, m_selectionStack.SelectionCount() > 0))
              {
                m_copiedAssets.CopyFrom(m_selectionStack);
              }
              
              if (ImGui::MenuItem("Paste", "Ctrl+V", nullptr, m_copiedAssets.SelectionCount() > 0))
              {
                PasteCopiedAssets();
              }
              
              if (ImGui::MenuItem("Duplicate", "Ctrl+D", nullptr, m_selectionStack.SelectionCount() > 0))
              {
                m_copiedAssets.CopyFrom(m_selectionStack);
                PasteCopiedAssets();
              }
              ImGui::EndPopup(); // Right click
            }
            ImGui::PopStyleVar(); // ItemSpacing
            
            // Main Area ---------------------------------------------------------------------------------------------
            const float paddingForOutline = 2.0f;
            const float scrollBarrOffset = 20.0f + ImGui::GetStyle().ScrollbarSize;
            float panelWidth = ImGui::GetContentRegionAvail().x - scrollBarrOffset;
            float cellSize = ApplicationSettings::Get().contentBrowserThumbnailSize + s_padding + paddingForOutline;
            int32_t columnCount = (int)(panelWidth / cellSize);
            if (columnCount < 1)
            {
              columnCount = 1;
            }
            
            // Render Items
            {
              const float rowSpacing = 12.0f;
              UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(paddingForOutline, rowSpacing));
              ImGui::Columns(columnCount, 0, false);
              
              UI::ScopedStyle border(ImGuiStyleVar_FrameBorderSize, 0.0f);
              UI::ScopedStyle padding(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
              RenderItems();
            }
            
            if (ImGui::IsWindowFocused() and !ImGui::IsMouseDragging(ImGuiMouseButton_Left))
            {
              UpdateInput();
            }
            
            ImGui::PopStyleColor(2);
            
            RenderDeleteDialogue();
          }
          ImGui::EndChild(); // Main Area
          
          if (ImGui::BeginDragDropTarget())
          {
            auto data = ImGui::AcceptDragDropPayload("scene_entity_hierarchy");
            if (data)
            {
            }
            ImGui::EndDragDropTarget();
          }
          
        }
        ImGui::EndChild(); // folders_common
        ImGui::EndTable();
      }
      UI::PopID();
    }
    ImGui::End();
  }
  
  void ContentBrowserPanel::RenderDeleteDialogue()
  {
    if (ImGui::BeginPopupModal("Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar))
    {
      {
        UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, 0);
        if (m_selectionStack.SelectionCount() == 0)
        {
          ImGui::CloseCurrentPopup();
        }
        
        ImGui::Text("Are you sure you want to delete %d items?", (int32_t)m_selectionStack.SelectionCount());
        
        float columnWidth = ImGui::GetContentRegionAvail().x / 4;
        
        ImGui::Columns(4, 0, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::SetColumnWidth(1, columnWidth);
        ImGui::SetColumnWidth(2, columnWidth);
        ImGui::SetColumnWidth(3, columnWidth);
        ImGui::NextColumn();
        if (ImGui::Button("Yes", ImVec2(columnWidth, 0)) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
        {
          for (AssetHandle handle : m_selectionStack)
          {
            size_t index = m_currentItems.FindItem(handle);
            if (index == ContentBrowserItemList::InvalidItem)
            {
              continue;
            }
            m_currentItems[index]->Delete();
            m_currentItems.Erase(handle);
          }
          
          for (AssetHandle handle : m_selectionStack)
          {
            if (m_directories.find(handle) != m_directories.end())
            {
              RemoveDirectory(m_directories[handle]);
            }
          }
          
          m_selectionStack.Clear();
          
          ChangeDirectory(m_currentDirectory);
          
          ImGui::CloseCurrentPopup();
        }
        
        ImGui::NextColumn();
        ImGui::SetItemDefaultFocus();
        if (ImGui::Button("No", ImVec2(columnWidth, 0)) or ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
        {
          ImGui::CloseCurrentPopup();
        }
        ImGui::NextColumn();
      }
      ImGui::EndPopup();
    }
  }
  
  void ContentBrowserPanel::RemoveDirectory(Ref<DirectoryInfo>& directory, bool removeFromParent)
  {
    if (directory->parent and removeFromParent)
    {
      auto& childList = directory->parent->subDirectories;
      childList.erase(childList.find(directory->handle));
    }
    
    for (auto&[handle, subdir] : directory->subDirectories)
    {
      RemoveDirectory(subdir, false);
    }
    
    directory->subDirectories.clear();
    directory->assets.clear();
    
    m_directories.erase(m_directories.find(directory->handle));
  }
  
  void ContentBrowserPanel::RenderItems()
  {
    m_isAnyItemHovered = false;
    bool openDeleteDialogue = false;
    
    for (auto& item : m_currentItems)
    {
      item->OnRenderBegin();
      CBItemActionResult result = item->OnRender();
      
      // Clear the selection
      if (result.IsSet(ContentBrowserAction::ClearSelections))
      {
        ClearSelections();
      }
      
      // Select the item
      if (result.IsSet(ContentBrowserAction::Selected) and !m_selectionStack.IsSelected(item->GetID()))
      {
        m_selectionStack.Select(item->GetID());
        item->SetSelected(true);
      }
      
      // Multiple Selection
      if (result.IsSet(ContentBrowserAction::SelectToHere) and m_selectionStack.SelectionCount() == 2)
      {
        size_t firstIndex = m_currentItems.FindItem(m_selectionStack[0]);
        size_t lastIndex = m_currentItems.FindItem(item->GetID());
        
        if (firstIndex > lastIndex)
        {
          size_t temp = firstIndex;
          firstIndex = lastIndex;
          lastIndex = temp;
        }
        
        for (size_t i = firstIndex; i <= lastIndex; i++)
        {
          auto toSelect = m_currentItems[i];
          toSelect->SetSelected(true);
          m_selectionStack.Select(toSelect->GetID());
        }
      }
      if (result.IsSet(ContentBrowserAction::StartRenaming))
      {
        item->StartRenaming();
      }
      
      if (result.IsSet(ContentBrowserAction::Copy))
      {
        m_copiedAssets.Select(item->GetID());
      }
      
      if (result.IsSet(ContentBrowserAction::Reload))
      {
        AssetManager::ReloadData(item->GetID());
      }
      
      if (result.IsSet(ContentBrowserAction::OpenDeleteDialogue))
      {
        openDeleteDialogue = true;
      }
      
      if (result.IsSet(ContentBrowserAction::Hovered))
      {
        m_isAnyItemHovered = true;
      }
      item->OnRenderEnd();
      
      if (result.IsSet(ContentBrowserAction::Renamed))
      {
        Refresh();
        SortItemList();
        item->SetSelected(true);
        m_selectionStack.Select(item->GetID());
        break;
      }
      
      if (result.IsSet(ContentBrowserAction::NavigateToThis))
      {
        ChangeDirectory(std::dynamic_pointer_cast<ContentBrowserDirectory>(item)->GetDirectoryInfo());
        break;
      }
      
      if (result.IsSet(ContentBrowserAction::Refresh))
      {
        Refresh();
        break;
      }
    }
    
    // This is a workaround an issue with ImGui: https://github.com/ocornut/imgui/issues/331
    if (openDeleteDialogue)
    {
      ImGui::OpenPopup("Delete");
      openDeleteDialogue = false;
    }
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
        fillWithColour(UI::Color::Selection);
      }
      else
      {
        const ImColor col = UI::ColorWithMultipliedValue(UI::Color::Selection, 0.8f);
        fillWithColour(UI::ColorWithMultipliedSaturation(col, 0.7f));
      }
      
      ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::BackgroundDark);
    }
    else if (isAnyDescendantSelected)
    {
      fillWithColour(UI::Color::SelectionMuted);
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
          const ImU32 buttonCol = UI::Color::BackgroundDark;
          const ImU32 buttonColP = UI::ColorWithMultipliedValue(UI::Color::BackgroundDark, 0.8f);
          UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonCol,
                                            ImGuiCol_ButtonHovered, buttonCol,
                                            ImGuiCol_ButtonActive, buttonColP);
          
          const float iconSize = std::min(24.0f, height);
          const float iconPadding = 3.0f;
          const bool clicked = ImGui::Button(labelId, ImVec2(iconSize, iconSize));
          UI::DrawButtonImage(icon, UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding),
                              UI::Color::TextDarker,
                              UI::ColorWithMultipliedValue(UI::Color::TextDarker, 1.2f),
                              UI::ColorWithMultipliedValue(UI::Color::TextDarker, 0.8f));
          
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
      
      // Search
      {
        UI::ShiftCursorY(2.0f);
        ImGui::SetNextItemWidth(200);
        if (UI::Widgets::SearchWidget(m_searchBuffer, MAX_INPUT_BUFFER_LENGTH))
        {
          if (strlen(m_searchBuffer) == 0)
          {
            ChangeDirectory(m_currentDirectory);
          }
          else
          {
            m_currentItems = Search(m_searchBuffer, m_currentDirectory);
            SortItemList();
          }
        }
        UI::ShiftCursorY(-2.0f);
      }
      
      if (m_updateNavigationPath)
      {
        m_breadCrumbData.clear();
        
        Ref<DirectoryInfo> current = m_currentDirectory;
        while (current and current->parent != nullptr)
        {
          m_breadCrumbData.push_back(current);
          current = current->parent;
        }
        
        std::reverse(m_breadCrumbData.begin(), m_breadCrumbData.end());
        m_updateNavigationPath = false;
      }
      
      // Breadcrumbs
      {
        UI::ScopedFont boldFont(UI::Font::Get(UI::FontType::Bold));
        UI::ScopedColor textColour(ImGuiCol_Text, UI::Color::TextDarker);
        
        const std::string& assetsDirectoryName = m_project->GetConfig().assetDirectory;
        ImVec2 textSize = ImGui::CalcTextSize(assetsDirectoryName.c_str());
        const float textPadding = ImGui::GetStyle().FramePadding.y;
        if (ImGui::Selectable(assetsDirectoryName.c_str(), false, 0, ImVec2(textSize.x, textSize.y + textPadding)))
        {
          ChangeDirectory(m_baseDirectory);
        }
        UpdateDropArea(m_baseDirectory);
        
        for (auto& directory : m_breadCrumbData)
        {
          ImGui::Text("/");
          
          std::string directoryName = directory->filePath.filename().string();
          ImVec2 textSize = ImGui::CalcTextSize(directoryName.c_str());
          if (ImGui::Selectable(directoryName.c_str(), false, 0, ImVec2(textSize.x, textSize.y + textPadding)))
          {
            ChangeDirectory(directory);
          }
          UpdateDropArea(directory);
        }
      }
      
      // Settings button
      ImGui::Spring();
      if (UI::Widgets::OptionsButton())
      {
        ImGui::OpenPopup("ContentBrowserSettings");
      }
      UI::SetTooltip("Content Browser settings");
      
      if (UI::BeginPopup("ContentBrowserSettings"))
      {
        auto& editorSettings = ApplicationSettings::Get();
        
        bool saveSettings = ImGui::MenuItem("Show Asset Types", nullptr, &editorSettings.contentBrowserShowAssetTypes);
        saveSettings |= ImGui::SliderInt("##thumbnail_size", &editorSettings.contentBrowserThumbnailSize, 32, 512);
        UI::SetTooltip("Thumnail Size");
        
        if (saveSettings)
        {
          ApplicationSettingsSerializer::SaveSettings();
        }
        
        UI::EndPopup();
      }
    }
    ImGui::EndHorizontal();
    ImGui::EndChild();
  }
  
  void ContentBrowserPanel::OnEvent([[maybe_unused]] Event &e)
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
        if (type == AssetType::Invalid)
        {
          continue;
        }
        
        // If that file is valid for Kreator then Import and push in registry
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
    if (filepath.string() == "" or filepath.string() == ".")
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
        Ref<Image> assetTexture = m_fileTex;
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
    }
  }
  
  void ContentBrowserPanel::OnBrowseBack()
  {
    m_nextDirectory = m_currentDirectory;
    m_previousDirectory = m_currentDirectory->parent;
    ChangeDirectory(m_previousDirectory);
  }
  
  void ContentBrowserPanel::OnBrowseForward()
  {
    ChangeDirectory(m_nextDirectory);
  }
  
  void ContentBrowserPanel::Refresh()
  {
    // Clear the Items
    m_currentItems.Clear();
    m_directories.clear();
    
    // Process the directories again
    Ref<DirectoryInfo> currentDirectory = m_currentDirectory;
    AssetHandle baseDirectoryHandle = ProcessDirectory(m_project->GetAssetDirectory().string(), nullptr);
    m_baseDirectory = m_directories[baseDirectoryHandle];
    m_currentDirectory = GetDirectory(currentDirectory->filePath);
    
    // Update current durectory
    if (!m_currentDirectory)
    {
      m_currentDirectory = m_baseDirectory; // Our current directory was removed
    }
    ChangeDirectory(m_currentDirectory);
  }
  
  ContentBrowserItemList ContentBrowserPanel::Search(const std::string &query, const Ref<DirectoryInfo> &directoryInfo)
  {
    ContentBrowserItemList results;
    std::string queryLowerCase = Utils::String::ToLower(query);
    
    // Directories
    for (auto&[handle, subdir] : directoryInfo->subDirectories)
    {
      std::string subdirName = subdir->filePath.filename().string();
      if (subdirName.find(queryLowerCase) != std::string::npos)
      {
        results.items.push_back(CreateRef<ContentBrowserDirectory>(subdir, m_folderIcon));
      }
      
      ContentBrowserItemList list = Search(query, subdir);
      results.items.insert(results.items.end(), list.items.begin(), list.items.end());
    }
    
    // Asset files
    for (auto& assetHandle : directoryInfo->assets)
    {
      auto& asset = AssetManager::GetMetadata(assetHandle);
      std::string filename = Utils::String::ToLower(asset.filePath.filename().string());
      
      if (filename.find(queryLowerCase) != std::string::npos)
      {
        // Set the asset texture
        Ref<Image> assetTexture = m_fileTex;
        if (m_assetIconMap.find(asset.filePath.extension().string()) != m_assetIconMap.end())
        {
          assetTexture = m_assetIconMap[asset.filePath.extension().string()];
        }
        
        // Create Item list for asset
        m_currentItems.items.push_back(CreateRef<ContentBrowserAsset>(asset, assetTexture));
      }
      if (queryLowerCase[0] != '.')
      {
        continue;
      }
      
      if (asset.filePath.extension().string().find(std::string(&queryLowerCase[1])) != std::string::npos)
      {
        // Set the asset texture
        Ref<Image> assetTexture = m_fileTex;
        if (m_assetIconMap.find(asset.filePath.extension().string()) != m_assetIconMap.end())
        {
          assetTexture = m_assetIconMap[asset.filePath.extension().string()];
        }
        
        // Create Item list for asset
        m_currentItems.items.push_back(CreateRef<ContentBrowserAsset>(asset, assetTexture));
      }
    }
    return results;
  }
  
  void ContentBrowserPanel::PasteCopiedAssets()
  {
    if (m_copiedAssets.SelectionCount() == 0)
    {
      return;
    }
    
    auto GetUniquePath = [](const std::filesystem::path& fp)
    {
      int counter = 0;
      auto checkFileName = [&counter, &fp](auto checkFileName) -> std::filesystem::path
      {
        ++counter;
        const std::string counterStr = [&counter] {
          if (counter < 10)
            return "0" + std::to_string(counter);
          else
            return std::to_string(counter);
        }();
        
        std::string basePath = Utils::String::RemoveExtension(fp.string()) + "_" + counterStr + fp.extension().string();
        if (std::filesystem::exists(basePath))
          return checkFileName(checkFileName);
        else
          return std::filesystem::path(basePath);
      };
      
      return checkFileName(checkFileName);
    };
    
    for (AssetHandle copiedAsset : m_copiedAssets)
    {
      const auto& item = m_currentItems[m_currentItems.FindItem(copiedAsset)];
      auto originalFilePath = Project::GetAssetDirectory();
      
      if (item->GetType() == ContentBrowserItem::ItemType::Asset)
      {
        originalFilePath /= std::dynamic_pointer_cast<ContentBrowserAsset>(item)->GetAssetInfo().filePath;
        auto filepath = GetUniquePath(originalFilePath);
        std::filesystem::copy_file(originalFilePath, filepath);
      }
      else
      {
        originalFilePath /= std::dynamic_pointer_cast<ContentBrowserDirectory>(item)->GetDirectoryInfo()->filePath;
        auto filepath = GetUniquePath(originalFilePath);
        std::filesystem::copy(originalFilePath, filepath, std::filesystem::copy_options::recursive);
      }
    }
    
    Refresh();
    m_selectionStack.Clear();
    m_copiedAssets.Clear();
  }
  
  void ContentBrowserPanel::UpdateInput()
  {
    if (!m_isHovered)
    {
      return;
    }
    
    if ((!m_isAnyItemHovered and ImGui::IsMouseDown(ImGuiMouseButton_Left)) or Input::IsKeyPressed(Key::Escape))
    {
      ClearSelections();
    }
    
    if (Input::IsKeyPressed(Key::Delete) and m_selectionStack.SelectionCount() > 0)
    {
      ImGui::OpenPopup("Delete");
    }
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

} // namespace Kreator
