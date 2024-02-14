//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Renderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Renderer3D.hpp"
#include "Renderer/TextRenderer.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/RendererAPI.hpp"
#include "Renderer/Graphics/Shader.hpp"

namespace IKan
{
  /// This structure stores the renderer data
  struct RendererData
  {
    RendererType rendererType = RendererType::Invalid;
    Scope<RendererAPI> rendererAPI;
  };
  static RendererData s_rendererData;

  void Renderer::Initialize()
  {
    IK_PROFILE();
    
    // Create the Renderer API instance
    s_rendererData.rendererAPI = RendererAPIFactory::Create();
    
    // Initialize the Renderers
    Renderer2D::Initialize({100, 100, 100});
    Renderer3D::Initialize();
    TextRenderer::Initialize();
    Font::Initialize();
  }
  
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    
    // Reset the Renderer API Data
    s_rendererData.rendererAPI.reset();
    s_rendererData.rendererAPI = nullptr;
    
    // Delete Shaders from library
    ShaderLibrary::Reset();
    
    // Shutdown the Renderers
    Renderer2D::Shutdown();
    Renderer3D::Shutdown();
    TextRenderer::Shutdown();
    Font::Shutdown();
  }
  
  // Renderer API ----------------------------------------------------------------------------------------------------
  void Renderer::SetCurrentRendererAPI(RendererType rendererType)
  {
    s_rendererData.rendererType = rendererType;
  }
  RendererType Renderer::GetCurrentRendererAPI()
  {
    return s_rendererData.rendererType;
  }
  
  // Renderer Controller ---------------------------------------------------------------------------------------------
  void Renderer::Clear(const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer::Clear");
    s_rendererData.rendererAPI->SetClearColor(color);
    s_rendererData.rendererAPI->ClearBits();
  }
  void Renderer::ClearBits()
  {
    IK_PERFORMANCE("Renderer::ClearBits");
    s_rendererData.rendererAPI->ClearBits();
  }
  void Renderer::ClearDepthBits()
  {
    IK_PERFORMANCE("Renderer::ClearDepthBits");
    s_rendererData.rendererAPI->ClearDepthBits();
  }
  void Renderer::EnableDepthPass()
  {
    IK_PERFORMANCE("Renderer::EnableDepthPass");
    s_rendererData.rendererAPI->EnableDepthPass();
  }
  void Renderer::DisableDepthPass()
  {
    IK_PERFORMANCE("Renderer::DisableDepthPass");
    s_rendererData.rendererAPI->DisableDepthPass();
  }
  void Renderer::DepthFunc(GlDepthFunc func)
  {
    IK_PERFORMANCE("Renderer::DepthFunc");
    s_rendererData.rendererAPI->DepthFunc(func);
  }
  void Renderer::EnableStencilPass()
  {
    IK_PERFORMANCE("Renderer::EnableStencilPass");
    s_rendererData.rendererAPI->EnableStencilPass();
  }
  void Renderer::DisableStencilPass()
  {
    IK_PERFORMANCE("Renderer::DisableStencilPass");
    s_rendererData.rendererAPI->DisableStencilPass();
  }
  void Renderer::SetViewport(uint32_t width, uint32_t height)
  {
    s_rendererData.rendererAPI->SetViewport(width, height);
  }
  
  // Draw Calls -----------------------------------------------------------------------------------------------------
  void Renderer::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    IK_PERFORMANCE("Renderer::DrawIndexed");
    s_rendererData.rendererAPI->DrawIndexed(pipeline, count);
  }
  void Renderer::DrawIndexedStrip(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    IK_PERFORMANCE("Renderer::DrawIndexedStrip");
    s_rendererData.rendererAPI->DrawIndexedStrip(pipeline, count);
  }
  
  void Renderer::DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount)
  {
    IK_PERFORMANCE("Renderer::DrawLines");
    s_rendererData.rendererAPI->DrawLines(pipeline, vertexCount);
  }
  
  void Renderer::DrawArrays(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    IK_PERFORMANCE("Renderer::DrawArrays");
    s_rendererData.rendererAPI->DrawArrays(pipeline, count);
  }
  
  void Renderer::DrawQuad(const Ref<Pipeline>& pipeline)
  {
    IK_PERFORMANCE("Renderer::DrawQuad");
    s_rendererData.rendererAPI->DrawQuad(pipeline);
  }
  
  void Renderer::DrawIndexedBaseVertex(uint32_t indexCount, void* indicesData, uint32_t baseVertex)
  {
    IK_PERFORMANCE("Renderer::DrawIndexedBaseVertex");
    s_rendererData.rendererAPI->DrawIndexedBaseVertex(indexCount, indicesData, baseVertex);
  }
  void Renderer::DrawFullscreenCube(const Ref<Pipeline>& pipeline)
  {
    IK_PERFORMANCE("Renderer::DrawFullscreenCube");
    s_rendererData.rendererAPI->DrawCube(pipeline);
  }

  // Renderer Capabilities -------------------------------------------------------------------------------------------
  RendererCapabilities& RendererCapabilities::Get()
  {
    static RendererCapabilities capabilities;
    return capabilities;
  }
} // namespace IKan
