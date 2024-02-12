//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
  Ref<Mesh> m;
  Ref<Shader> s;
  
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
    
    m = Mesh::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/IKan/Assets/Meshes/Default/Cube.fbx");
    s = ShaderFactory::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
  }
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
    m.reset();
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    Renderer::Clear({0.2f, 0.22f, 0.222f, 1.0f});
    
    Renderer2D::BeginBatch(Utils::Math::UnitMat4, Utils::Math::UnitMat4);
    Renderer2D::DrawQuad({0.1, 0.2, 0.3}, Utils::Math::UnitVec2, Utils::Math::ZeroVec3, {0.2, 0.3, 0.3, 1.0});
    Renderer2D::DrawCircle({0, 0, 0}, 1.0f);
    Renderer2D::EndBatch();
    
    TextRenderer::BeginBatch(Utils::Math::UnitMat4);
    TextRenderer::RenderText("Sample Text", {-0.8, 0.6, 0}, {0.2, 0.2}, {1, 1, 1, 1});
    TextRenderer::EndBatch();
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
