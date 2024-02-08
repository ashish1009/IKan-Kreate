//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
  KreatorLayer::KreatorLayer()
  : Layer("Kreator Renderer")
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Attaching '{0} Layer' to application", GetName());
    
    // Testing Temp Shader
    Ref<Shader> shader = ShaderLibrary::GetShader("/Users/ashish./iKan_storage/Github/Product/Kreator/IKan/Assets/Shaders/PBR_StaticShader.glsl");
  }
  
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    Renderer::Clear({0.2f, 0.22f, 0.222f, 1.0f});
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
