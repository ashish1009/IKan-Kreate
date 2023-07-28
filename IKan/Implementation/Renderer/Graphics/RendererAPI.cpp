//
//  RendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "RendererAPI.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  Scope<RendererAPI> RendererAPI::Create()
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Render API not Supporting");
    }
  }
} // namespace IKan
