//
//  KreatorConsolePanel.hpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
//

#pragma once

namespace Kreator
{
  class KreatorConsolePanel : public EditorConsolePanel
  {
  public:
    /// Kreator Console panel constructor
    KreatorConsolePanel();
    /// Kreator Console panel default destructor
    ~KreatorConsolePanel() = default;
    
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function renders the menu bar of Panel
    void RenderMenu();
    /// This function renders the consol
    void RenderConsole();
    
    // Member Variables ----------------------------------------------------------------------------------------------
    bool m_displayMessageInspector = false;
    bool m_isMessageInspectorHovered = false;
    ConsoleMessage* m_selectedMessage = nullptr;
    
    Ref<Texture> m_infoButtonTex, m_warningButtonTex, m_errorButtonTex;
    inline static ImGuiTextFilter s_searchFilter;
  };
} // namespace Kreator
