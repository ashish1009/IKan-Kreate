//
//  RendererAPI.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "RendererAPI.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace IKan
{
  Scope<RendererAPI> RendererAPI::Create()
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateScope<OpenGLRendererAPI>();
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Render API not Supporting");
    }
  }
} // namespace IKan
