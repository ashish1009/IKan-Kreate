//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  RendererLayer* RendererLayer::s_instance = nullptr;
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer()
  : Layer("Kreator Renderer")
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_INFO("RendererLayer", "Creating '{0} Layer' instance", GetName());
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("RendererLayer", "Destroying '{0} Layer' instance", GetName());
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Attaching '{0} Layer' to application", GetName());
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Detaching '{0} Layer' from application", GetName());
  }
  
  void RendererLayer::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    Renderer::Clear({0.2, 0.1, 0.3, 1.0});
    
    Renderer2D::BeginBatch(Utils::Math::UnitMat4, Utils::Math::UnitMat4);
    Renderer2D::SubmitQuad({0.5, 0.5, 0}, {0.5, 0.5});
    Renderer2D::SubmitCircle({-0.5, -0.5, 0}, 0.5);
    Renderer2D::EndBatch();
  }
  
  void RendererLayer::OnEvent([[maybe_unused]] Event& event)
  {
    
  }
  
  void RendererLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
