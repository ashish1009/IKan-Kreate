//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Renderer.hpp"

namespace IKan
{
  /// This structure stores the renderer data
  struct RendererData
  {
    RendererType rendererType = RendererType::Invalid;
  };
  static RendererData s_rendererData;

  void Renderer::Initialize()
  {
    IK_PROFILE();
  }
  
  void Renderer::Shutdown()
  {
    IK_PROFILE();
  }
  
  void Renderer::SetCurrentRendererAPI(RendererType rendererType)
  {
    s_rendererData.rendererType = rendererType;
  }
  
  RendererType Renderer::GetCurrentRendererAPI()
  {
    return s_rendererData.rendererType;
  }
} // namespace IKan
