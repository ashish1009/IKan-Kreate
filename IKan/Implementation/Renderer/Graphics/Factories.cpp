//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Factories.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererContext.hpp"

namespace IKan
{
  Scope<RendererContext> RendererContextFactory::Create(GLFWwindow* windowPtr)
  {
    switch(Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL:
        return CreateScope<OpenGLRendererContext>(windowPtr);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL("[Graphics Factory]", "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
        IK_ASSERT(false, "Invalid Renderer API");
    }
  }
} // namespace IKan
