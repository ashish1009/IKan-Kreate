//
//  CoreWindow.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "CoreWindow.hpp"
#include "Platform/MacOS/MacWindow.hpp"

namespace IKan
{
  Ref<Window> Window::Create(const Specification& specification)
  {
#ifdef __APPLE__
    return CreateRef<MacWindow>(specification);
#else
    IK_ASSERT(false, "Invalid Operating system");
#endif
  }
} // namespace IKan
