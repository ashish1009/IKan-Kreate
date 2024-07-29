//
//  AssetPanel.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator
{
  class AssetPanel : public EditorPanel
  {
  public:
    /// This is the consturctor to create Project Setting Window instance
    AssetPanel() = default;
    /// This is the Project Setting window destructor
    ~AssetPanel() = default;
    
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;
  };
} // namespace Kreator
