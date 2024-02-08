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
  
  void Renderer::SetCurrentRendererAPI(RendererType rendererType)
  {
    s_rendererData.rendererType = rendererType;
  }
  
  RendererType Renderer::GetCurrentRendererAPI()
  {
    return s_rendererData.rendererType;
  }
  
  // Renderer Capabilities -------------------------------------------------------------------------------------------
  RendererCapabilities& RendererCapabilities::Get()
  {
    static RendererCapabilities capabilities;
    return capabilities;
  }
} // namespace IKan
