//
//  ContentBrowserItem.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ContentBrowserItem.hpp"

namespace Kreator
{
  static char s_renameBuffer[MAX_INPUT_BUFFER_LENGTH];
  
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
