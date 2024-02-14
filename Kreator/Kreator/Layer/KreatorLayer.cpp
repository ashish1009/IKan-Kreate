//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

namespace Kreator
{
#define TEST_MESH 1
#if TEST_MESH
  Ref<Mesh> m;
  Ref<Shader> s;
#endif
  
  static EditorCamera camera{45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f};

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
    
    FixedCamera::SetViewport(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
  }
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());

#if TEST_MESH
    m.reset();
#endif
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    camera.OnUpdate(ts);
    camera.SetActive(true);
    
    Renderer::Clear({0.2f, 0.2f, 0.2f, 1.0f});

    Renderer2D::BeginBatch(camera.GetUnReversedViewProjection(), camera.GetViewMatrix());
    Renderer2D::DrawQuad({-2, 2, 3}, Utils::Math::UnitVec2, Utils::Math::ZeroVec3, {0.2, 0.3, 0.3, 1.0});
    Renderer2D::DrawCircle({0, 0, 0}, 1.0f);
    Renderer2D::EndBatch();
    
    TextRenderer::BeginBatch(camera.GetUnReversedViewProjection());
    TextRenderer::RenderText("Sample Text", {-0.8, 0.6, 0}, {0.2, 0.2}, {1, 1, 1, 1});
    TextRenderer::EndBatch();
    
#if TEST_MESH
    s->Bind();
    s->SetUniformMat4("u_ViewProjection", camera.GetUnReversedViewProjection());
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
    
    {
      IK_PERFORMANCE("KreatorLayer::RenderSystemInfo");
      static constexpr glm::vec3 position = { 5.0f, 5.0f, 0.3f };
      static constexpr glm::vec2 size = {0.3f, 0.3f};
      static constexpr glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1.0f};
      
      TextRenderer::BeginBatch(FixedCamera::s_projection);
      TextRenderer::RenderFixedViewText("(c) IKAN", { Application::Get().GetWindow().GetWidth()  - 80, 5.0f, 0.3f }, size, color, Font::GetDefaultFont());
      TextRenderer::RenderFixedViewText(std::to_string((uint32_t)(ImGui::GetIO().Framerate)), position, size, color, Font::GetDefaultFont());
      TextRenderer::EndBatch();
    }

  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    camera.OnEvent(event);
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
