//
//  ContentBrowserPanel.hpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#pragma once

namespace Kreator
{
  class ContentBrowserPanel : public EditorPanel
  {
  public:
    /// Default Constructor
    ContentBrowserPanel();
    
    /// @see Editor Panel for Documentation
    void OnImGuiRender(bool& isOpen) override;
    /// @see Editor Panel for Documentation
    void OnEvent(Event& e) override;
    /// @see Editor Panel for Documentation
    void OnProjectChanged(const Ref<Project>& project) override;
  };
} // namespace Kreator
