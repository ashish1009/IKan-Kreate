//
//  Factories.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Factories.hpp"

#include "Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLRendererContext.hpp"
#include "Platform/OpenGL/OpenGLRendererAPI.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "Platform/OpenGL/OpenGLRendererBuffer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLPipeline.hpp"

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
  
  Scope<RendererAPI> RendererAPIFactory::Create()
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateScope<OpenGLRendererAPI>();
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  
  Ref<Texture> TextureFactory::Create(const Texture2DSpecification& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLTexture>(spec);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  Ref<Texture> TextureFactory::Create(uint32_t data)
  {
    // TODO: IKan: Copy inside Open GL Texture class?
    static uint32_t whiteTextureData = data;
    
    // Texture specification
    Texture2DSpecification textureSpec;
    textureSpec.title = "White texture";
    textureSpec.width = 1;
    textureSpec.height = 1;
    textureSpec.data = &whiteTextureData;
    textureSpec.size = sizeof(uint32_t);
    
    return Create(textureSpec);
  }
  Ref<Texture> TextureFactory::CreateWhiteTexture()
  {
    return Create(0xffffffff);
  }
  
  Ref<VertexBuffer> VertexBufferFactory::Create(void *data, uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLVertexBuffer>(data, size);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  
  Ref<VertexBuffer> VertexBufferFactory::Create(uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  
  Ref<IndexBuffer> IndexBufferFactory::CreateWithSize(void *data, uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, size);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  
  Ref<IndexBuffer> IndexBufferFactory::CreateWithCount(void *data, uint32_t count)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count * SizeOfSingleIndices);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  Ref<Shader> ShaderFactory::Create(const std::filesystem::path& shaderFilePath)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLShader>(shaderFilePath);
      case RendererType::Invalid:
      default:
        IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                        "Call Renderer::SetCurrentRendererAPI(RendererType) before any Renderer Initialization to set Renderer API type."
                        "'RendererType should not be RendererType::Invalid'");
        IK_ASSERT(false , "Renderer API type is not set!")
    }
    return nullptr;
  }
  
  Ref<Pipeline> PipelineFactory::Create(const PipelineSpecification& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLPipeline>(spec);
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
