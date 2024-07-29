//
//  ContentBrowserItem.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ContentBrowserItem.hpp"

#include "Editor/ApplicationSettings.hpp"
#include "Panels/ContentBrowserPanel.hpp"
#include "Layers/RendererLayer.hpp"

namespace Kreator
{
  static char s_renameBuffer[MAX_INPUT_BUFFER_LENGTH];
  
  void CBItemActionResult::Set(ContentBrowserAction flag, bool value)
  {
    if (value)
    {
      Field |= (uint16_t)flag;
    }
    else
    {
      Field &= ~(uint16_t)flag;
    }
  }
  
  bool CBItemActionResult::IsSet(ContentBrowserAction flag) const
  {
    return (uint16_t)flag & Field;
  }
  
  ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle id, const std::string& name, const Ref<Image>& icon)
  : m_type(type), m_ID(id), m_name(name), m_icon(icon)
  {
    
  }
  
  void ContentBrowserItem::OnRenderBegin()
  {
    ImGui::PushID(&m_ID);
    ImGui::BeginGroup();
  }
  
  CBItemActionResult ContentBrowserItem::OnRender()
  {
    IK_PERFORMANCE("ContentBrowserItem::OnRender");
    CBItemActionResult result;
    
    const auto& editorSettings = ApplicationSettings::Get();
    const float thumbnailSize = editorSettings.contentBrowserThumbnailSize;
    const bool displayAssetType = editorSettings.contentBrowserShowAssetTypes;
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    const float edgeOffset = 4.0f;
    const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
    const float infoPanelHeight = std::max(displayAssetType ? thumbnailSize * 0.5f : textLineHeight, textLineHeight);
    
    const ImVec2 topLeft = ImGui::GetCursorScreenPos();
    const ImVec2 thumbBottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize };
    const ImVec2 infoTopLeft = { topLeft.x,         topLeft.y + thumbnailSize };
    const ImVec2 bottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize + infoPanelHeight };
    
    // Lamda to draw shadow
    auto drawShadow = [](const ImVec2& topLeft, const ImVec2& bottomRight, bool directory)
    {
      auto* drawList = ImGui::GetWindowDrawList();
      const ImRect itemRect = UI::RectOffset(ImRect(topLeft, bottomRight), 1.0f, 1.0f);
      drawList->AddRect(itemRect.Min, itemRect.Max,
                        UI::Color::PropertyField, 6.0f, directory ? 0 : ImDrawFlags_RoundCornersBottom, 2.0f);
    };
    
    // Fill background
    //----------------
    if (m_type != ItemType::Directory)
    {
      auto* drawList = ImGui::GetWindowDrawList();
      
      // Draw shadow
      drawShadow(topLeft, bottomRight, false);
      
      // Draw background
      drawList->AddRectFilled(topLeft, thumbBottomRight, UI::Color::BackgroundDark);
      drawList->AddRectFilled(infoTopLeft, bottomRight, UI::Color::GroupHeader, 6.0f, ImDrawFlags_RoundCornersBottom);
    }
    else if (ImGui::ItemHoverable(ImRect(topLeft, bottomRight), ImGui::GetID(&m_ID)) or m_isSelected)
    {
      // If hovered or selected directory
      // Draw shadow
      drawShadow(topLeft, bottomRight, true);
      auto* drawList = ImGui::GetWindowDrawList();
      drawList->AddRectFilled(topLeft, bottomRight, UI::Color::GroupHeader, 6.0f);
    }
    
    // Thumbnail
    //==========
    // TODO: replace with actual Asset Thumbnail interface
    ImGui::InvisibleButton("##thumbnailButton", ImVec2{ thumbnailSize, thumbnailSize });
    UI::DrawButtonImage(m_icon, UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f),
                        IM_COL32(255, 255, 255, 225),
                        IM_COL32(255, 255, 255, 255),
                        IM_COL32(255, 255, 255, 255));
    
    // Info Panel
    //-----------
    auto renamingWidget = [&]
    {
      ImGui::SetKeyboardFocusHere();
      if (ImGui::InputText("##rename", s_renameBuffer, MAX_INPUT_BUFFER_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
      {
        Rename(s_renameBuffer);
        m_isRenaming = false;
        result.Set(ContentBrowserAction::Renamed, true);
      }
    };
    
    UI::ShiftCursor(edgeOffset, edgeOffset);
    if (m_type == ItemType::Directory)
    {
      ImGui::BeginVertical((std::string("InfoPanel") + m_name).c_str(),
                           ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
      {
        // Centre align directory name
        ImGui::BeginHorizontal(m_name.c_str(), ImVec2(thumbnailSize - 2.0f, 0.0f));
        ImGui::Spring();
        {
          ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));
          const float textWidth = std::min(ImGui::CalcTextSize(m_name.c_str()).x, thumbnailSize);
          if (m_isRenaming)
          {
            ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
            renamingWidget();
          }
          else
          {
            ImGui::SetNextItemWidth(textWidth);
            ImGui::Text(m_name.c_str());
          }
          ImGui::PopTextWrapPos();
        }
        ImGui::Spring();
        ImGui::EndHorizontal();
        
        ImGui::Spring();
      }
      ImGui::EndVertical();
    }
    else
    {
      ImGui::BeginVertical((std::string("InfoPanel") + m_name).c_str(),
                           ImVec2(thumbnailSize - edgeOffset * 3.0f, infoPanelHeight - edgeOffset));
      {
        ImGui::BeginHorizontal("label", ImVec2(0.0f, 0.0f));
        
        ImGui::SuspendLayout();
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 2.0f));
        if (m_isRenaming)
        {
          ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
          renamingWidget();
        }
        else
        {
          ImGui::Text(m_name.c_str());
        }
        ImGui::PopTextWrapPos();
        ImGui::ResumeLayout();
        
        ImGui::Spring();
        ImGui::EndHorizontal();
      }
      ImGui::Spring();
      
      if (displayAssetType)
      {
        ImGui::BeginHorizontal("assetType", ImVec2(0.0f, 0.0f));
        ImGui::Spring();
        {
          const AssetMetadata& metadata = AssetManager::GetMetadata(m_ID);
          const std::string& assetType = Utils::String::ToUpper(AssetUtils::AssetTypeToString(metadata.type));
          UI::ScopedColor textColor(ImGuiCol_Text, UI::Color::TextDarker);
          ImGui::TextUnformatted(assetType.c_str());
        }
        ImGui::EndHorizontal();
        
        ImGui::Spring(-1.0f, edgeOffset);
      }
      ImGui::EndVertical();
    }
    UI::ShiftCursor(-edgeOffset, -edgeOffset);
    
    if (!m_isRenaming)
    {
      if (Input::IsKeyPressed(Key::F2) and m_isSelected)
      {
        StartRenaming();
      }
    }
    
    ImGui::PopStyleVar();
    
    // End of the Item Group
    //======================
    ImGui::EndGroup();
    
    // Draw outline
    //-------------
    if (m_isSelected or ImGui::IsItemHovered())
    {
      ImRect itemRect = UI::GetItemRect();
      auto* drawList = ImGui::GetWindowDrawList();
      
      if (m_isSelected)
      {
        const bool mouseDown = ImGui::IsMouseDown(ImGuiMouseButton_Left) and ImGui::IsItemHovered();
        ImU32 colTransition = UI::ColorWithMultipliedValue(UI::Color::Selection, 0.8f);
        
        drawList->AddRect(itemRect.Min, itemRect.Max,
                          mouseDown ? colTransition : UI::Color::Selection, 6.0f,
                          m_type == ItemType::Directory ? 0 : ImDrawFlags_RoundCornersBottom, 1.0f);
      }
      else // isHovered
      {
        if (m_type != ItemType::Directory)
        {
          drawList->AddRect(itemRect.Min, itemRect.Max,
                            UI::Color::Muted, 6.0f,
                            ImDrawFlags_RoundCornersBottom, 1.0f);
        }
      }
    }
    
    // Mouse Events handling
    //======================
    UpdateDrop(result);
    
    // Drag the Item
    bool dragging = false;
    if (dragging = ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID); dragging)
    {
      m_isDragging = true;
      
      const auto& selectionStack = ContentBrowserPanel::Get().GetSelectionStack();
      if (!selectionStack.IsSelected(m_ID))
      {
        result.Set(ContentBrowserAction::ClearSelections, true);
      }
      
      auto& currentItems = ContentBrowserPanel::Get().GetCurrentItems();
      
      if (selectionStack.SelectionCount() > 0)
      {
        for (const auto& selectedItemHandles : selectionStack)
        {
          size_t index = currentItems.FindItem(selectedItemHandles);
          if (index == ContentBrowserItemList::InvalidItem)
          {
            continue;
          }
          
          const auto& item = currentItems[index];
          UI::Image(item->GetIcon(), ImVec2(20, 20));
          ImGui::SameLine();
          const auto& name = item->GetName();
          ImGui::TextUnformatted(name.c_str());
        }
        
        ImGui::SetDragDropPayload("asset_payload", selectionStack.SelectionData(), sizeof(AssetHandle) * selectionStack.SelectionCount());
      }
      
      result.Set(ContentBrowserAction::Selected, true);
      ImGui::EndDragDropSource();
    }
    
    // Item Hovered
    if (ImGui::IsItemHovered())
    {
      result.Set(ContentBrowserAction::Hovered, true);
      
      if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
      {
        Activate(result);
      }
      else
      {
        const auto& selectionStack = ContentBrowserPanel::Get().GetSelectionStack();
        
        bool action = selectionStack.SelectionCount() > 1 ?
        ImGui::IsMouseReleased(ImGuiMouseButton_Left) :
        ImGui::IsMouseClicked(ImGuiMouseButton_Left);
        
        bool skipBecauseDragging = m_isDragging and selectionStack.IsSelected(m_ID);
        if (action and !skipBecauseDragging)
        {
          result.Set(ContentBrowserAction::Selected, true);
          
          if (!Input::IsKeyPressed(Key::LeftControl) and!Input::IsKeyPressed(Key::LeftShift))
          {
            result.Set(ContentBrowserAction::ClearSelections, true);
          }
          
          if (Input::IsKeyPressed(Key::LeftShift))
          {
            result.Set(ContentBrowserAction::SelectToHere, true);
          }
        }
      }
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
    if (ImGui::BeginPopupContextItem("CBItemContextMenu"))
    {
      result.Set(ContentBrowserAction::Selected, true);
      
      if (!Input::IsKeyPressed(Key::LeftControl) and !Input::IsKeyPressed(Key::LeftShift))
      {
        result.Set(ContentBrowserAction::ClearSelections, true);
      }
      
      if (Input::IsKeyPressed(Key::LeftShift))
      {
        result.Set(ContentBrowserAction::SelectToHere, true);
      }
      
      OnContextMenuOpen(result);
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    
    m_isDragging = dragging;
    
    return result;
  }
  
  void ContentBrowserItem::OnRenderEnd()
  {
    ImGui::PopID();
    ImGui::NextColumn();
  }
  
  void ContentBrowserItem::StartRenaming()
  {
    IK_PROFILE();
    if (m_isRenaming)
    {
      return;
    }
    memset(s_renameBuffer, 0, MAX_INPUT_BUFFER_LENGTH);
    memcpy(s_renameBuffer, m_name.c_str(), m_name.size());
    m_isRenaming = true;
  }
  
  void ContentBrowserItem::StopRenaming()
  {
    IK_PROFILE();
    m_isRenaming = false;
    memset(s_renameBuffer, 0, MAX_INPUT_BUFFER_LENGTH);
  }
  
  void ContentBrowserItem::SetSelected(bool value)
  {
    m_isSelected = value;
  }
  
  void ContentBrowserItem::Rename(const std::string& newName)
  {
    OnRenamed(newName);
  }
  
  AssetHandle ContentBrowserItem::GetID() const
  {
    return m_ID;
  }
  ContentBrowserItem::ItemType ContentBrowserItem::GetType() const
  {
    return m_type;
  }
  const std::string& ContentBrowserItem::GetName() const
  {
    return m_name;
  }
  const Ref<Image>& ContentBrowserItem::GetIcon() const
  {
    return m_icon;
  }
  bool ContentBrowserItem::IsSelected() const
  {
    return m_isSelected;
  }
  bool ContentBrowserItem::IsRenaming() const
  {
    return m_isRenaming;
  }
  
  void ContentBrowserItem::OnRenamed(const std::string& newName)
  {
    m_name = newName;
  }
  
  void ContentBrowserItem::OnContextMenuOpen(CBItemActionResult& actionResult)
  {
    if (ImGui::MenuItem("Reload"))
    {
      actionResult.Set(ContentBrowserAction::Reload, true);
    }
    
    if (ImGui::MenuItem("Rename"))
    {
      actionResult.Set(ContentBrowserAction::StartRenaming, true);
    }
    
    if (ImGui::MenuItem("Copy"))
    {
      actionResult.Set(ContentBrowserAction::Copy, true);
    }
    
    if (ImGui::MenuItem("Delete"))
    {
      actionResult.Set(ContentBrowserAction::OpenDeleteDialogue, true);
    }
    
    RenderCustomContextItems();
  }
  
  ContentBrowserDirectory::ContentBrowserDirectory(const Ref<DirectoryInfo>& directoryInfo, const Ref<Image>& icon)
  : ContentBrowserItem(ContentBrowserItem::ItemType::Directory, directoryInfo->handle,
                       directoryInfo->filePath.filename().string(), icon), m_directoryInfo(directoryInfo)
  {
    
  }
  
  void ContentBrowserDirectory::Activate(CBItemActionResult& actionResult)
  {
    actionResult.Set(ContentBrowserAction::NavigateToThis, true);
  }
  
  void ContentBrowserDirectory::OnRenamed(const std::string& newName)
  {
    if (std::filesystem::exists(Project::GetActive()->GetAssetDirectory() / m_directoryInfo->filePath.parent_path() / newName))
    {
      IK_LOG_ERROR("ContentBrowser", "A directory with that name already exists!");
      return;
    }
    
    Utils::FileSystem::Rename(Project::GetActive()->GetAssetDirectory() / m_directoryInfo->filePath,
                              Project::GetActive()->GetAssetDirectory() / m_directoryInfo->filePath.parent_path() / newName);
  }
  
  void ContentBrowserDirectory::UpdateDrop([[maybe_unused]]CBItemActionResult& actionResult)
  {
    if (IsSelected())
    {
      return;
    }
    
    if (ImGui::BeginDragDropTarget())
    {
      // TODO: Implement later
      ImGui::EndDragDropTarget();
    }
  }
  
  void ContentBrowserDirectory::Delete()
  {
    bool deleted = Utils::FileSystem::Delete(Project::GetActive()->GetAssetDirectory() / m_directoryInfo->filePath);
    if (!deleted)
    {
      IK_LOG_ERROR("ContentBrowser", "Failed to delete folder {0}", m_directoryInfo->filePath.string().c_str());
      return;
    }
    
    for (auto asset : m_directoryInfo->assets)
    {
      AssetManager::OnAssetDeleted(asset);
    }
  }
  
  bool ContentBrowserDirectory::Move(const std::filesystem::path& destination)
  {
    bool wasMoved = Utils::FileSystem::MoveFile(Project::GetActive()->GetAssetDirectory() / m_directoryInfo->filePath,
                                                Project::GetActive()->GetAssetDirectory() / destination);
    if (!wasMoved)
    {
      return false;
    }
    return true;
  }
  
  ContentBrowserAsset::ContentBrowserAsset(const AssetMetadata& assetInfo, const Ref<Image>& icon)
  : ContentBrowserItem(ContentBrowserItem::ItemType::Asset, assetInfo.handle, assetInfo.filePath.stem().string(), icon),
  m_assetInfo(assetInfo)
  {
    
  }
  
  void ContentBrowserAsset::Delete()
  {
    auto filepath = AssetManager::GetFileSystemPath(m_assetInfo);
    bool deleted = Utils::FileSystem::Delete(filepath);
    if (!deleted)
    {
      IK_LOG_ERROR("ContentBrowser", "Couldn't delete {0}", m_assetInfo.filePath.string().c_str());
      return;
    }
    
    Ref<DirectoryInfo> currentDirectory = ContentBrowserPanel::Get().GetDirectory(m_assetInfo.filePath.parent_path());
    currentDirectory->assets.erase(std::remove(currentDirectory->assets.begin(),
                                               currentDirectory->assets.end(),
                                               m_assetInfo.handle),
                                   currentDirectory->assets.end());
    
    AssetManager::OnAssetDeleted(m_assetInfo.handle);  }
  
  bool ContentBrowserAsset::Move(const std::filesystem::path& destination)
  {
    auto filepath = AssetManager::GetFileSystemPath(m_assetInfo);
    bool wasMoved = Utils::FileSystem::MoveFile(filepath, Project::GetActive()->GetAssetDirectory() / destination);
    if (!wasMoved)
    {
      IK_LOG_ERROR("ContentBrowser", "Couldn't move {0} to {1}",
                   m_assetInfo.filePath.string().c_str(), destination.string().c_str());
      return false;
    }
    
    AssetManager::OnAssetMoved(m_assetInfo.handle, destination);
    return true;  }
  
  void ContentBrowserAsset::Activate([[maybe_unused]] CBItemActionResult& actionResult)
  {
    IK_ASSERT(false, "Implement later");
  }
  
  void ContentBrowserAsset::OnRenamed(const std::string& newName)
  {
    auto filepath = AssetManager::GetFileSystemPath(m_assetInfo);
    std::filesystem::path newFilepath = fmt::format("{0}/{1}{2}", filepath.parent_path().string(),
                                                    newName, filepath.extension().string());
    
    if (Utils::FileSystem::Rename(filepath, newFilepath))
    {
      // Update AssetManager with new name
      [[maybe_unused]] auto& metadata = AssetManager::GetMetadata(m_assetInfo.handle);
      AssetManager::OnAssetRenamed(m_assetInfo.handle, newFilepath);
    }
    else
    {
      IK_LOG_ERROR("ContentBrowser", "A file with that name already exists!");
    }
  }
} // namespace Kreator
