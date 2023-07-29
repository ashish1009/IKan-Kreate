//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  // TODO: Temp
  static Ref<Image> m_image;
  
  RendererLayer::RendererLayer()
  : Layer("Kreator Renderer"), m_editorCamera(45.0f, 1280.0f, 720.0f, 0.1f, 1000.0f)
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
    
    // TODO: Temp
    {
      m_editorScene = Scene::Create();
      m_image = Image::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/Kreator/Resources/Textures/checkerboard.png");
    }
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
    
    // TODO: Temp
    {
      m_image.reset();
    }
  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    // TODO: Temp
    {
      m_editorCamera.SetActive(true);
      m_editorCamera.OnUpdate(ts);

      Renderer2D::BeginRenderPass();
      Renderer::Clear({0.12f, 0.12f, 0.18f, 1.0f});
      
      Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection());
      
      Renderer2D::DrawQuad({1, 1, 0}, {1, 1, 1}, {0, 0, 0}, m_image);
      Renderer2D::DrawCircle({0, 0, 0});
      Renderer2D::DrawRect({0, 0, 0}, {4, 4}, {1, 0, 0.5, 1});
      Renderer2D::RenderText("Sample Test Text", Font::GetDefaultFont(), {0, -1, 0}, {1, 1}, {1, 1, 1, 1}, -1);
      
      Renderer2D::EndBatch();
      Renderer2D::EndRenderPass();
    }
  }
  
  void RendererLayer::OnEvent(Event& event)
  {
    m_editorCamera.OnEvent(event);
  }
  
  void RendererLayer::OnImguiRender()
  {
    IK_PERFORMANCE("RendererLayer::OnImguiRender");
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
        
    auto viewportSize = ImGui::GetContentRegionAvail();
        
    // Render viewport image
    ImGui::Image(INT2VOIDP(Renderer2D::GetFinalImage()->GetRendererID()), viewportSize,
                 {0, 1}, {1, 0});
    
    ImGui::End();
    ImGui::PopStyleVar();

  }
} // namespace Kreator
