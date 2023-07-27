//
//  RendererContext.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "RendererContext.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  Scope<RendererContext> RendererContext::Create(GLFWwindow *window_ptr)
  {
    switch(Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API which is not supported by IKan engine" );
    }
  }
} // namespace IKan
