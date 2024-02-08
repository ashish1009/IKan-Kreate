//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Factories.hpp"
#include "Platform/OpenGL/OpenGLRendererContext.hpp"

namespace IKan
{
  Scope<RendererContext> RendererContextFactory::Create(GLFWwindow* windowPtr)
  {
    return nullptr;
  }
} // namespace IKan
