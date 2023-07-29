//
//  RenderPass.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "RenderPass.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRenderPass.hpp"

namespace IKan
{
  Ref<RenderPass> RenderPass::Create(const Specification &spec)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
} // namespace IKan
