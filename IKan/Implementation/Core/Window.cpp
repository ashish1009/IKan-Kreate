//
//  Window.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Window.hpp"
#include "Platform/MacOS/MacWindow.hpp"

namespace IKan
{
  Scope<Window> WindowFactory::Create(const WindowSpecification &windowSpec)
  {
#ifdef __APPLE__
    return CreateScope<MacWindow>(windowSpec);
#else
    IK_FORCE_ASSERT(false, "Invalid Operating system");
#endif
  }
} // namespace IKan
