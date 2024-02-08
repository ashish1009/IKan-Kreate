//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Renderer.hpp"
#include "Renderer/Graphics/RendererAPI.hpp"

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
  }
  
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    
    // Reset the Renderer API Data
    s_rendererData.rendererAPI.reset();
    s_rendererData.rendererAPI = nullptr;
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
    s_rendererData.rendererAPI->ClearDepthBits();
  }
  void Renderer::EnableDepthPass()
  {
    s_rendererData.rendererAPI->EnableDepthPass();
  }
  void Renderer::DisableDepthPass()
  {
    s_rendererData.rendererAPI->DisableDepthPass();
  }
  void Renderer::DepthFunc(GlDepthFunc func)
  {
    s_rendererData.rendererAPI->DepthFunc(func);
  }
  void Renderer::EnableStencilPass()
  {
    s_rendererData.rendererAPI->EnableStencilPass();
  }
  void Renderer::DisableStencilPass()
  {
    s_rendererData.rendererAPI->DisableStencilPass();
  }
  void Renderer::SetViewport(uint32_t width, uint32_t height)
  {
    s_rendererData.rendererAPI->SetViewport(width, height);
  }

  // Renderer Capabilities -------------------------------------------------------------------------------------------
  RendererCapabilities& RendererCapabilities::Get()
  {
    static RendererCapabilities capabilities;
    return capabilities;
  }
} // namespace IKan
