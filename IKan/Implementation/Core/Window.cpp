//
//  Window.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "Window.hpp"
#include "Platform/MacOS/MacWindow.hpp"

namespace IKan
{
  Ref<Window> WindowFactory::Create(const WindowSpecification &windowSpec)
  {
#ifdef __APPLE__
    return CreateRef<MacWindow>(windowSpec);
#else
    IK_ASSERT(false, "Invalid Operating system");
#endif
  }
} // namespace IKan
