//
//  PanelManager.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

#include "Editor/EditorPanel.hpp"

namespace IKan
{
  using PanelID = uint32_t;
  
  /// Stores the Panel data of each editor panels
  struct PanelData
  {
    const char* ID = "";
    const char* name = "";
    Ref<EditorPanel> panel;
    bool isOpen = false;
  };
} // namespace IKan
