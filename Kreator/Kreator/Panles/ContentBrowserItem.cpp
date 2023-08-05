//
//  ContentBrowserItem.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserItem.hpp"
#include "ContentBrowserPanel.hpp"

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
                                         const Ref<Texture>& icon)
  : m_type(type), m_ID(id), m_name(name), m_icon(icon)
  {
    
  }
  
  void ContentBrowserItem::OnRenderBegin()
  {
    IK_ASSERT(false);
  }
  
  CBItemActionResult ContentBrowserItem::OnRender()
  {
    CBItemActionResult result;
    IK_ASSERT(false);
    return result;
  }
  
  void ContentBrowserItem::OnRenderEnd()
  {
    IK_ASSERT(false);
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
  
  ContentBrowserDirectory::ContentBrowserDirectory(const Ref<DirectoryInfo>& directoryInfo, const Ref<Texture>& icon)
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
} // namespace Kreator
