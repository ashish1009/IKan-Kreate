//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ContentBrowserPanel.hpp"

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
} // namespace Kreator
