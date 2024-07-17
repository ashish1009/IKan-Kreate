//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/07/24.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  Ref<Mesh> meshCube;
  Ref<Material> meshMaterial;
  Ref<Image> texImage;

  RendererLayer* RendererLayer::s_instance = nullptr;
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer()
  : Layer("Kreator Renderer"), m_viewportRenderer("Primary Viewport"), m_miniViewportRenderer("Mini Viewport"),
  m_secondaryViewportRenderer("Secondary Viewport")
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
    
    meshCube = Mesh::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/Kreator/Projects/3D/TestProject/Assets/Meshes/Default/Cube.fbx");
    meshMaterial = Material::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
    texImage = TextureFactory::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/Kreator/Outputs/WelcomeScreen.png");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Detaching '{0} Layer' from application", GetName());
    
    meshCube.reset();
    meshMaterial.reset();
    texImage.reset();
  }
  
  void RendererLayer::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    m_editorCamera.OnUpdate(ts);
    m_editorCamera.SetActive(true);

    m_viewportRenderer.BeginScene();
    m_viewportRenderer.EndScene();
    Renderer::Clear({0.2, 0.1, 0.3, 1.0});
        
    Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
    Renderer2D::SubmitQuad({0.5, 0.5, 0}, {0.5, 0.5}, {0, 0, 0}, {1, 1, 1, 1}, texImage);
    Renderer2D::SubmitRect({0.0, 0.0, 0}, {0.5, 0.5});
    Renderer2D::SubmitText("Sample Text", {0, 0, 0}, {0.1, 0.1});
    Renderer2D::SubmitCircle({-0.5, -0.5, 0}, 0.5);
    Renderer2D::EndBatch();
    
    meshCube->GetPipeline()->Bind();
    static glm::mat4 transform = Utils::Math::UnitMat4;
    meshMaterial->Set("u_ViewProjection", m_editorCamera.GetUnReversedViewProjection());
    for (const SubMesh& submesh : meshCube->GetSubMeshes())
    {
      meshMaterial->Set("u_Transform", transform * submesh.transform);
      meshMaterial->Bind();
      Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
    } 
  }
  
  void RendererLayer::OnEvent([[maybe_unused]] Event& event)
  {
    m_editorCamera.OnEvent(event);
  }
  
  void RendererLayer::OnImGuiRender()
  {
    // Docked Windows-----------
    UI_StartMainWindowDocking();
    UI_Viewport();
        
    // Should be rendered last inside docker
    UI_StatisticsPanel();

    UI_EndMainWindowDocking();
  }
  
  void RendererLayer::UpdateViewportSize()
  {
    IK_PROFILE()
        
    FixedCamera::SetViewport(m_primaryViewport.width, m_primaryViewport.height);
    
    m_editorCamera.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    
    m_viewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_miniViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
    m_secondaryViewportRenderer.SetViewportSize(m_primaryViewport.width, m_primaryViewport.height);
  }
  
  // UI APIs ---------------------------------------------------------------------------------------------------------
  void RendererLayer::UI_StartMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_StartMainWindowDocking");
    static bool optFullscreenPersistant = true;
    static ImGuiDockNodeFlags optFlags = ImGuiDockNodeFlags_None;
    bool optFullscreen = optFullscreenPersistant;
    
    // ImGui Input Output Fonts
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    
    io.ConfigWindowsResizeFromEdges = io.BackendFlags & ImGuiBackendFlags_HasMouseCursors;
    
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
    if (optFullscreen)
    {
      ImGuiViewport* viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
      windowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 3.0f);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
    ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
    ImGui::PopStyleColor(); // MenuBarBg
    ImGui::PopStyleVar(2);
    
    if (optFullscreen)
    {
      ImGui::PopStyleVar(2);
    }
    
    // Dockspace
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 250.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
      ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
      ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), optFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
  }
  
  void RendererLayer::UI_EndMainWindowDocking()
  {
    IK_PERFORMANCE("RendererLayer::UI_EndMainWindowDocking");
    ImGui::End();
  }
  
  void RendererLayer::UI_Viewport()
  {
    IK_PERFORMANCE("RendererLayer::UI_Viewport")
    
    UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    m_primaryViewport.isHovered = ImGui::IsWindowHovered();
    m_primaryViewport.isFocused = ImGui::IsWindowFocused();

    const ImVec2& offset = ImGui::GetCursorPos();
    const ImVec2& size = ImGui::GetContentRegionAvail();

    // Updating the View port size and kreator data w.r.t. primary viewport
    bool zeroSizeViewport = 0 == size.x or 0 == size.y;
    bool sameSizeViewport = size.x == m_primaryViewport.width and size.x == m_primaryViewport.height;
    if(!zeroSizeViewport and !sameSizeViewport)
    {
      m_primaryViewport.width = static_cast<uint32_t>(size.x);
      m_primaryViewport.height = static_cast<uint32_t>(size.y);
      
      // Set Viewport of Dummy Data
      UpdateViewportSize();
    }
    
    // Render viewport image
    UI::Image(texImage, size);

    ImGui::End();
  }
  
  void RendererLayer::UI_StatisticsPanel()
  {
    IK_PERFORMANCE("RendererLayer::UI_StatisticsPanel");
    if (ImGui::Begin("Statistics"))
    {
      const auto& perFrameData = PerformanceProfiler::Get().GetPerFrameData();
      for (auto&& [name, time] : perFrameData)
      {
        ImGui::Text("%s : %f", name, time);
      }

      ImGui::End();
    }
  }
} // namespace Kreator
