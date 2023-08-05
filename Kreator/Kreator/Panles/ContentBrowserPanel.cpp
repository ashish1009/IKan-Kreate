//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserPanel.hpp"

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
  
  ContentBrowserPanel::ContentBrowserPanel()
  {
    IK_ASSERT(false);
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
    IK_ASSERT(false);

  }
  
  AssetHandle ContentBrowserPanel::ProcessDirectory(const std::filesystem::path &directoryPath, const Ref<DirectoryInfo> &parent)
  {
    IK_ASSERT(false);

  }
  
  Ref<DirectoryInfo> ContentBrowserPanel::GetDirectory(const std::filesystem::path& filepath) const
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::ChangeDirectory(Ref<DirectoryInfo> &directory)
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::SortItemList()
  {
    IK_ASSERT(false);
  }
  
  void ContentBrowserPanel::ClearSelections()
  {
    IK_ASSERT(false);
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
