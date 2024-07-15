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
    
    meshCube = Mesh::Create("/Users/ashish./iKan_storage/Github/Product/Kreator/Kreator/Projects/3D/TestProject/Assets/Meshes/Default/Cube.fbx");
    meshMaterial = Material::Create("/Users/ashish./iKan_storage/Github/Product/IKan-Kreate/IKan/Assets/Shaders/PBR_StaticShader.glsl");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("RendererLayer", "Detaching '{0} Layer' from application", GetName());
    
    meshCube.reset();
    meshMaterial.reset();
  }
  
  void RendererLayer::OnUpdate([[maybe_unused]] TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    
    m_editorCamera.OnUpdate(ts);
    m_editorCamera.SetActive(true);

    Renderer::Clear({0.2, 0.1, 0.3, 1.0});
        
    Renderer2D::BeginBatch(m_editorCamera.GetUnReversedViewProjection(), m_editorCamera.GetViewMatrix());
    Renderer2D::SubmitQuad({0.5, 0.5, 0}, {0.5, 0.5});
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
    
  }
} // namespace Kreator
