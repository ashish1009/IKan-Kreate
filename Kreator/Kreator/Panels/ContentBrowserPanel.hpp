//
//  ContentBrowserPanel.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "ContentBrowserItem.hpp"

namespace Kreator
{
  /// This class stores the selection Items in a stack
  class SelectionStack : public VectorIterator<AssetHandle>
  {
  public:
    /// Default constructor
    SelectionStack();
    
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
    
  private:
    std::vector<AssetHandle> m_selections;
  };
} // namespace Kreator
