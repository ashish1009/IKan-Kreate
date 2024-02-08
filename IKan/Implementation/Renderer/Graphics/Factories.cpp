//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Factories.hpp"
#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererContext.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"

namespace IKan
{
  Scope<RendererContext> RendererContextFactory::Create(GLFWwindow* windowPtr)
  {
    switch(Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateScope<OpenGLRendererContext>(windowPtr);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Scope<RendererAPI> RendererAPIFactory::Create()
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateScope<OpenGLRendererAPI>();
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<Shader> ShaderFactory::Create(const std::filesystem::path& shaderFilePath)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLShader>(shaderFilePath);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
} // namespace IKan
