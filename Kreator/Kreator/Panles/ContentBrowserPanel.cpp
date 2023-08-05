//
//  ContentBrowserPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserPanel.hpp"

namespace Kreator
{
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
} // namespace IKan
