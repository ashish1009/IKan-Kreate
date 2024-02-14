//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
#define TEST_MESH 0
#if TEST_MESH
  Ref<Mesh> m;
  Ref<Shader> s;
#endif
  
  static Ref<SceneCamera> sc;

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
    
#if TEST_MESH
    m = Mesh::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/IKan/Assets/Meshes/Default/Cube.fbx");
    s = ShaderFactory::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
#endif
    sc = CreateRef<SceneCamera>();
    
    Utils::Math::Print("Scene Camera Projecttion Matrix" ,sc->GetProjectionMatrix());
  }
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());

#if TEST_MESH
    m.reset();
#endif
    sc.reset();
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    Renderer::Clear({0.2f, 0.22f, 0.222f, 1.0f});
    
    Renderer2D::BeginBatch(sc->GetUnReversedProjectionMatrix(), Utils::Math::UnitMat4);
    Renderer2D::DrawQuad({0.1, 0.2, 0.3}, Utils::Math::UnitVec2, Utils::Math::ZeroVec3, {0.2, 0.3, 0.3, 1.0});
    Renderer2D::DrawCircle({0, 0, 0}, 1.0f);
    Renderer2D::EndBatch();
    
    TextRenderer::BeginBatch(sc->GetProjectionMatrix());
    TextRenderer::RenderText("Sample Text", {-0.8, 0.6, 0}, {0.2, 0.2}, {1, 1, 1, 1});
    TextRenderer::EndBatch();
    
#if TEST_MESH
    s->Bind();
    s->SetUniformMat4("u_ViewProjection", Utils::Math::UnitMat4);
    s->SetUniformMat4("u_Transform", Utils::Math::UnitMat4);
    
    m->GetPipeline()->Bind();
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(glm::vec3(30, 30, 30))));
    auto t = glm::translate(glm::mat4(1.0f), Utils::Math::ZeroVec3) * rotation * glm::scale(glm::mat4(1.0f), Utils::Math::UnitVec3);

    for (const auto& sm : m->GetSubMeshes())
    {
      s->SetUniformMat4("u_Transform", t * sm.transform);
      Renderer::DrawIndexedBaseVertex(sm.indexCount, (void*)(sizeof(uint32_t) * sm.baseIndex), sm.baseVertex);
    }
#endif
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
