//
//  ContentBrowserItem.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserItem.hpp"
#include "ContentBrowserPanel.hpp"
#include "ApplicationSettings.hpp"

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
  
  
  ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle id, const std::string& name,
                                         const Ref<Image>& icon)
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
    CBItemActionResult result;

    const auto& editorSettings = ApplicationSettings::Get();
    const float thumbnailSize = editorSettings.ContentBrowserThumbnailSize;
    const bool displayAssetType = editorSettings.ContentBrowserShowAssetTypes;

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
                        UI::Theme::Color::PropertyField, 6.0f, directory ? 0 : ImDrawFlags_RoundCornersBottom, 2.0f);
    };

    // Fill background
    //----------------
    if (m_type != ItemType::Directory)
    {
      auto* drawList = ImGui::GetWindowDrawList();
      
      // Draw shadow
      drawShadow(topLeft, bottomRight, false);
      
      // Draw background
      drawList->AddRectFilled(topLeft, thumbBottomRight, UI::Theme::Color::BackgroundDark);
      drawList->AddRectFilled(infoTopLeft, bottomRight, UI::Theme::Color::GroupHeader, 6.0f, ImDrawFlags_RoundCornersBottom);
    }
    else if (ImGui::ItemHoverable(ImRect(topLeft, bottomRight), ImGui::GetID(&m_ID)) or m_isSelected)
    {
      // If hovered or selected directory
      // Draw shadow
      drawShadow(topLeft, bottomRight, true);
      auto* drawList = ImGui::GetWindowDrawList();
      drawList->AddRectFilled(topLeft, bottomRight, UI::Theme::Color::GroupHeader, 6.0f);
    }
    
    // Thumbnail
    //==========
    // TODO: replace with actual Asset Thumbnail interface
    ImGui::InvisibleButton("##thumbnailButton", ImVec2{ thumbnailSize, thumbnailSize });
    UI::DrawButtonImage(m_icon, IM_COL32(255, 255, 255, 225),
                        IM_COL32(255, 255, 255, 255),
                        IM_COL32(255, 255, 255, 255),
                        UI::RectExpanded(UI::GetItemRect(), -6.0f, -6.0f));

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
          UI::ScopedColor textColor(ImGuiCol_Text, Kreator_UI::Color::TextDarker);
          ImGui::TextUnformatted(assetType.c_str());
        }
        ImGui::EndHorizontal();
        
        ImGui::Spring(-1.0f, edgeOffset);
      }
      ImGui::EndVertical();
    }
    UI::ShiftCursor(-edgeOffset, -edgeOffset);
    
    ImGui::PopStyleVar();

    // End of the Item Group
    //======================
    ImGui::EndGroup();
    return result;
  }
  
  void ContentBrowserItem::OnRenderEnd()
  {
    ImGui::PopID();
    ImGui::NextColumn();
  }
  
  void ContentBrowserItem::StartRenaming()
  {
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
  const Ref<Texture>& ContentBrowserItem::GetIcon() const
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
    IK_ASSERT(false);
  }
  
  void ContentBrowserDirectory::OnRenamed(const std::string& newName)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserDirectory::UpdateDrop(CBItemActionResult& actionResult)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserDirectory::Delete()
  {
    IK_ASSERT(false);
  }
  
  bool ContentBrowserDirectory::Move(const std::filesystem::path& destination)
  {
    IK_ASSERT(false);
  }
  
  ContentBrowserAsset::ContentBrowserAsset(const AssetMetadata& assetInfo, const Ref<Image>& icon)
  : ContentBrowserItem(ContentBrowserItem::ItemType::Asset, assetInfo.handle, assetInfo.filePath.stem().string(), icon),
  m_assetInfo(assetInfo)
  {
    
  }
  
  void ContentBrowserAsset::Delete()
  {
    IK_ASSERT(false);
  }
  
  bool ContentBrowserAsset::Move(const std::filesystem::path& destination)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserAsset::Activate(CBItemActionResult& actionResult)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserAsset::OnRenamed(const std::string& newName)
  {
    IK_ASSERT(false);
  }

} // namespace Kreator
