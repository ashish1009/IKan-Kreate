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
  void Popup::SetFlag(bool flag)
  {
    m_openFlag = flag;
  }
  
  void Popup::SetSize(float width, float height)
  {
    m_width = width;
    m_height = height;
  }
  
  void Popup::SetCenter(bool center)
  {
    m_center = center;
  }
  
  void Popup::Set(std::string_view title, bool openFlag, float width, float height, bool center)
  {
    m_title = title;
    SetFlag(openFlag);
    SetSize(width, height);
    SetCenter(center);
  }
} // namespace Kreator::UI
