//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
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
      case RendererType::OpenGL: return CreateScope<OpenGLRendererContext>(windowPtr);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
} // namespace IKan
