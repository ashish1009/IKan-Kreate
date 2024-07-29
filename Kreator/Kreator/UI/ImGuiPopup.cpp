//
//  ImGuiPopup.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ImGuiPopup.hpp"

namespace Kreator::UI
{
  Popup::Popup (std::string_view title, bool openFlag, float width, float height, bool center)
  : m_title(title), m_openFlag(openFlag), m_width(width), m_height(height), m_center(center)
  {
    
  }
} // namespace Kreator::UI
