//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Factories.hpp"

#include "Renderer/Renderer.hpp"

namespace IKan
{
  Scope<RendererContext> RendererContextFactory::Create(GLFWwindow* windowPtr)
  {
    return nullptr;
  }
} // namespace IKan
