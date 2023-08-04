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
    /// Kreator Console panel destructor
    ~KreatorConsolePanel();
    
    /// @see Editor Panel
    virtual void OnImguiRender(bool& isOpen) override;
  };
} // namespace Kreator
