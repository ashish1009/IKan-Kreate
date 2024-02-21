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
#include "Platform/OpenGL/OpenGLRendererBuffer.hpp"
#include "Platform/OpenGL/OpenGLPipeline.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

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
  
  Ref<VertexBuffer> VertexBufferFactory::Create(void *data, uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLVertexBuffer>(data, size);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<VertexBuffer> VertexBufferFactory::Create(uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<IndexBuffer> IndexBufferFactory::CreateWithSize(void *data, uint32_t size)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, size);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<IndexBuffer> IndexBufferFactory::CreateWithCount(void *data, uint32_t count)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLIndexBuffer>(data, count * SizeOfSingleIndices);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<Pipeline> PipelineFactory::Create(const PipelineSpecification& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLPipeline>(spec);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }

  Ref<Texture> TextureFactory::Create(const Texture2DSpecification& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLTexture>(spec);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<Texture> TextureFactory::Create(uint32_t data)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL:
      {
        // White data
        static uint32_t whiteTextureData = data;
        
        // Texture specificaion
        Texture2DSpecification textureSpec;
        textureSpec.width = 1;
        textureSpec.height = 1;
        textureSpec.data = &whiteTextureData;
        textureSpec.size = sizeof(uint32_t);
        
        return CreateRef<OpenGLTexture>(textureSpec);
      }
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<Texture> TextureFactory::Create(const std::filesystem::path& filePath)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL:
      {
        ImageSpecificaion spec;
        spec.filePath = filePath;
        return CreateRef<OpenGLImage>(spec);
      }
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<Texture> TextureFactory::Create(const ImageSpecificaion& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLImage>(spec);
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }
  
  Ref<FrameBuffer> FrameBufferFactory::Create(const FrameBufferSpecification& spec)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLFrameBuffer>(spec); 
      case RendererType::Invalid:
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }

  Ref<CharTexture> CharTextureFactory::Create(const FT_Face &face, const glm::ivec2 &size, const glm::ivec2 &bearing,
                                              uint32_t advance, [[maybe_unused]] char charVal)
  {
    switch (Renderer::GetCurrentRendererAPI())
    {
      case RendererType::OpenGL: return CreateRef<OpenGLCharTexture>(face, size, bearing, advance, charVal);
      default: IK_ASSERT(false, "Renderer API is not set or set as Invalid. (Renderer::SetRendererAPI(RendererType))");
    }
  }

} // namespace IKan
