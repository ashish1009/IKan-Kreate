//
//  ContentBrowserItem.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "ContentBrowserItem.hpp"

namespace Kreator
{  
  void CBItemActionResult::Set(ContentBrowserAction flag, bool value)
  {
    if (value)
    {
      Field |= (uint16_t)flag;
    }
    else
    {
      Field &= ~(uint16_t)flag;
    }
  }
  
  bool CBItemActionResult::IsSet(ContentBrowserAction flag) const
  {
    return (uint16_t)flag & Field;
  }
} // namespace Kreator
