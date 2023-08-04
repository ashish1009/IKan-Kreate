//
//  KreatorConsolePanel.hpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

namespace Kreator
{
  using namespace IKan;
  
  class KreatorConsolePanel : public EditorConsolePanel
  {
  public:
    /// Kreator Console panel constructor
    KreatorConsolePanel();
    /// Kreator Console panel default destructor
    ~KreatorConsolePanel() = default;
    
    /// @see Editor Panel
    virtual void OnImguiRender(bool& isOpen) override;
    
  private:
    Ref<Image> m_infoButtonTex, m_warningButtonTex, m_errorButtonTex;
  };
} // namespace Kreator
