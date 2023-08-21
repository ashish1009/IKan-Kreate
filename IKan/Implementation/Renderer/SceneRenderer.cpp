//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#include "SceneRenderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include <glad/glad.h>

namespace IKan
{
  bool SceneRenderer::MeshKey::operator<(const MeshKey &other) const
  {
    return meshHandle < other.meshHandle;
  }
  
  SceneRenderer::SceneRenderer(Ref<Scene> scene, const Renderer2DData& rendere2DData)
  : m_scene(scene)
  {
    if (!s_commonData)
    {
      s_commonData = CreateScope<SceneRendererData>();
      
      // Create the Render pass
      RenderPass::Specification rendererPassSpec;
      rendererPassSpec.debugName = "Renderer 2D";
      
      // Create Framebuffer for Render Pass
      FrameBuffer::Specification fbSpec;
      fbSpec.attachments =
      {
        FrameBuffer::Attachments::TextureFormat::RGBA8,
        FrameBuffer::Attachments::TextureFormat::R32I,
        FrameBuffer::Attachments::TextureFormat::Depth24Stencil
      };
      
      rendererPassSpec.targetFramebuffer = FrameBuffer::Create(fbSpec);
      s_commonData->renderPass = RenderPass::Create(rendererPassSpec);
    }
    
    Initialize(rendere2DData);
  }
  
  SceneRenderer::~SceneRenderer()
  {
    Renderer2D::Shutdown();
    
    // Destroy the Render Pass for Renderer 2D
    if (s_commonData)
    {
      s_commonData.reset();
    }
  }
  
  void SceneRenderer::Initialize(const Renderer2DData& rendere2DData)
  {
    // Initialize the Renderer Data
    Renderer2D::Initialize(rendere2DData);    
  }
  
  void SceneRenderer::SetViewport(uint32_t width, uint32_t height)
  {
    if (s_commonData->viewportWidth != width or s_commonData->viewportHeight != height)
    {
      s_commonData->needResize = true;
      s_commonData->viewportWidth = width;
      s_commonData->viewportHeight = height;
    }
    else
    {
      s_commonData->needResize = false;
    }
  }

  void SceneRenderer::BeginScene(const glm::mat4& camViewProjMat)
  {
    if (s_commonData->needResize)
    {
      // Resize the framebuffer
      s_commonData->renderPass->Resize(s_commonData->viewportWidth, s_commonData->viewportHeight);
    }
    
    s_commonData->camViewProjection = camViewProjMat;
    Renderer2D::BeginBatch(camViewProjMat);
  }
  
  void SceneRenderer::SubmitMeshSource(Ref<MeshSource> mesh)
  {
    MeshKey meshKey = { mesh->handle };
    auto& dc = m_meshSourceDrawList[meshKey];
    dc.meshSource = mesh;
  }
  
  void SceneRenderer::EndScene()
  {
    // 2D ---------------------
    Renderer2D::EndBatch();
    
    // 3D ---------------------
    FlushDrawList();
  }
  
  void SceneRenderer::FlushDrawList()
  {
    if (s_commonData->viewportWidth > 0 and s_commonData->viewportHeight > 0)
    {
      GeometryPass();
    }
  }
  
  void SceneRenderer::GeometryPass()
  {
    int i = 0;
    for (const auto& [mk, dc] : m_meshSourceDrawList)
    {
      auto pipeline = dc.meshSource->GetPipeline();
      pipeline->Bind();
      
      auto shader = pipeline->GetSpecification().shader;
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", s_commonData->camViewProjection);
      
      for (const SubMesh& submesh : dc.meshSource->GetSubMeshes())
      {
        shader->SetUniformMat4("u_Transform", Utils::Math::GetTransformMatrix({i*3, 0, 0}) * submesh.transform);
        Renderer::DrawIndexedBaseVertex(submesh.indexCount,
                                        (void*)(sizeof(uint32_t) * submesh.baseIndex),
                                        submesh.baseVertex);
      } // for (SubMesh& submesh : submeshes_)

      i++;
    }
  }
  
  void SceneRenderer::BeginRenderPass()
  {
    s_commonData->renderPass->Begin();
  }
  
  void SceneRenderer::EndRenderPass()
  {
    s_commonData->renderPass->End();
  }
  
  Ref<RenderPass> SceneRenderer::GetRenderPass()
  {
    return s_commonData->renderPass;
  }
  
  Ref<Texture> SceneRenderer::GetFinalImage()
  {
    // FIXME: (IKan) Use Final Image ID in Render Pass Specificaion. For now by deafult its 0 in all shaders
    return s_commonData->renderPass->GetSpecification().targetFramebuffer->GetColorAttachments().at(0);
  }
  
  void SceneRenderer::GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixeldData)
  {
    Renderer::GetEntityIdFromPixels(mx, my, s_commonData->renderPass->GetSpecification().targetFramebuffer->GetPixelIdIndex(), pixeldData);
  }
} // namespace IKan
