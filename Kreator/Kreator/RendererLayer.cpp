//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  RendererLayer::RendererLayer()
  : Layer("Kreator Renderer")
  {
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    
  }
  
  void RendererLayer::OnImguiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImguiRender");
  }
} // namespace Kreator
