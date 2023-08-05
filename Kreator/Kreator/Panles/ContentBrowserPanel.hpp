//
//  ContentBrowserPanel.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
#define MAX_INPUT_BUFFER_LENGTH 128
  
  /// This class stores the selection Items in a stack
  class SelectionStack
  {
  public:
    /// This fucntion copy the selection
    /// - Parameter other: other selection stack
    void CopyFrom(const SelectionStack& other);
    /// This function select a item
    /// - Parameter handle: item handle
    void Select(AssetHandle handle);
    /// This function deselect a item
    /// - Parameter handle: item handle
    void Deselect(AssetHandle handle);
    /// This function clears the item selection
    void Clear();
    /// This function checks if item is selectd
    /// - Parameter handle: item handle
    bool IsSelected(AssetHandle handle) const;
    /// This function returns the selection counts
    size_t SelectionCount() const;
    /// This function returns the selection data
    const AssetHandle* SelectionData() const;
    /// This function returns the item at index
    AssetHandle operator[](size_t index) const;
    
    std::vector<AssetHandle>::iterator begin() { return m_selections.begin(); }
    std::vector<AssetHandle>::const_iterator begin() const { return m_selections.begin(); }
    std::vector<AssetHandle>::iterator end() { return m_selections.end(); }
    std::vector<AssetHandle>::const_iterator end() const { return m_selections.end(); }
    
  private:
    std::vector<AssetHandle> m_selections;
  };
  
  /// This structure stores the content browser panel item in list
  struct ContentBrowserItemList
  {
    static constexpr size_t InvalidItem = std::numeric_limits<size_t>::max();
    std::vector<Ref<ContentBrowserItem>> items;
    
    // Iterators  ----------------------------------------------------------------------------------------------------
    std::vector<Ref<ContentBrowserItem>>::iterator begin() { return items.begin(); }
    std::vector<Ref<ContentBrowserItem>>::iterator end() { return items.end(); }
    std::vector<Ref<ContentBrowserItem>>::const_iterator begin() const { return items.begin(); }
    std::vector<Ref<ContentBrowserItem>>::const_iterator end() const { return items.end(); }
    
    // Operators ----------------------------------------------------------------------------------------------------
    Ref<ContentBrowserItem>& operator[](size_t index) { return items[index]; }
    const Ref<ContentBrowserItem>& operator[](size_t index) const { return items[index]; }
    
    /// This function clears the items
    void Clear();
    /// This function erases the item
    /// - Parameter handle: Item Handle to be removed
    void Erase(AssetHandle handle);
    /// This function searches the item from list
    /// - Parameter handle: Item Handle
    size_t FindItem(AssetHandle handle) const;
  };
  
} // namespace Kreator
