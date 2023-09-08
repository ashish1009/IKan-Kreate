//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#include "SceneRenderer.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include <glad/glad.h>

namespace IKan
{
  SceneRenderer::SceneRenderer(Ref<Scene> scene)
  : m_scene(scene)
  {
    if (!m_commonData)
    {
      m_commonData = CreateScope<SceneRendererData>();
      
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
      m_commonData->renderPass = RenderPass::Create(rendererPassSpec);
      
      m_commonData->stencilShader = Shader::Create(CoreAssetPath("Shaders/MeshStencilShader.glsl"));
    }
  }
  
  SceneRenderer::~SceneRenderer()
  {
    // Destroy the Render Pass for Renderer 2D
    if (m_commonData)
    {
      m_commonData.reset();
    }
  }
  
  void SceneRenderer::SetViewport(uint32_t width, uint32_t height)
  {
    if (m_commonData->viewportWidth != width or m_commonData->viewportHeight != height)
    {
      m_commonData->needResize = true;
      m_commonData->viewportWidth = width;
      m_commonData->viewportHeight = height;
    }
    else
    {
      m_commonData->needResize = false;
    }
  }

  void SceneRenderer::BeginScene(const glm::mat4& camViewProjMat, float camDistance)
  {
    if (m_commonData->needResize)
    {
      // Resize the framebuffer
      m_commonData->renderPass->Resize(m_commonData->viewportWidth, m_commonData->viewportHeight);
    }
    m_commonData->camViewProjection = camViewProjMat;
    m_commonData->cameraDistance = camDistance;
  }
  
  void SceneRenderer::SubmitMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform)
  {
    if (!mesh)
    {
      return;
    }
      
    MeshSourceDrawCommand dc;
    dc.staticMesh = mesh;
    dc.transform = transform;
    
    m_meshSourceDrawList.emplace_back(dc);
  }
  
  void SceneRenderer::SubmitSelectedMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform)
  {
    if (!mesh)
    {
      return;
    }
    
    MeshSourceDrawCommand dc;
    dc.staticMesh = mesh;
    dc.transform = transform;
    
    m_selectedMeshSourceDrawList.emplace_back(dc);
  }
  
  void SceneRenderer::EndScene()
  {
    FlushDrawList();
    ClearDrawLists();
  }
  
  void SceneRenderer::FlushDrawList()
  {
    if (m_commonData->viewportWidth > 0 and m_commonData->viewportHeight > 0)
    {
      GeometryPass();
      CompositePass();
    }
  }
  
  void SceneRenderer::ClearDrawLists()
  {
    m_meshSourceDrawList.clear();
    m_selectedMeshSourceDrawList.clear();
  }
  
  void SceneRenderer::RenderMesh(const std::vector<SubMesh>& submeshes, const Ref<Pipeline>& pipeline,
                                 const Ref<Shader>& shader, const glm::mat4& transform)
  {
    pipeline->Bind();
    shader->Bind();
    shader->SetUniformMat4("u_ViewProjection", m_commonData->camViewProjection);
    
    for (const SubMesh& submesh : submeshes)
    {
      shader->SetUniformMat4("u_Transform", transform * submesh.transform);
      Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
    } // for each submeshes
  }
  
  void SceneRenderer::GeometryPass()
  {
    for (const auto& dc : m_meshSourceDrawList)
    {
      const auto& pipeline = dc.staticMesh->GetPipeline();
      RenderMesh(dc.staticMesh->GetSubMeshes(), pipeline, pipeline->GetSpecification().shader, dc.transform);
    }
  }
  
  void SceneRenderer::CompositePass()
  {
    for (const auto& dc : m_selectedMeshSourceDrawList)
    {
      auto pipeline = dc.staticMesh->GetPipeline();
      // Stencil Pass Mesh Render
      {
        Renderer::EnableStencilPass();

        // Hack to change size of mesh
        glm::vec3 p, r, s;
        Utils::Math::DecomposeTransform(dc.transform, p, r, s);
        float scaleFactor = Utils::Math::FloatClamp(0.003 * m_commonData->cameraDistance, 0.005, 0.05);
        s += scaleFactor;
        auto tt = Utils::Math::GetTransformMatrix(p, r, s);

        RenderMesh(dc.staticMesh->GetSubMeshes(), pipeline, m_commonData->stencilShader, tt);
        
        Renderer::DisableStencilPass();
      }

      // Geometry pass Selected Mesh
      RenderMesh(dc.staticMesh->GetSubMeshes(), pipeline, pipeline->GetSpecification().shader, dc.transform);
    }
  }
  
  void SceneRenderer::BeginRenderPass()
  {
    m_commonData->renderPass->Begin();
  }
  
  void SceneRenderer::EndRenderPass()
  {
    m_commonData->renderPass->End();
  }
  
  Ref<RenderPass> SceneRenderer::GetRenderPass()
  {
    return m_commonData->renderPass;
  }
  
  Ref<Texture> SceneRenderer::GetFinalImage()
  {
    // FIXME: (IKan) Use Final Image ID in Render Pass Specificaion. For now by deafult its 0 in all shaders
    return m_commonData->renderPass->GetSpecification().targetFramebuffer->GetColorAttachments().at(0);
  }
  
  void SceneRenderer::GetEntityIdFromPixels(int32_t mx, int32_t my, int32_t& pixeldData)
  {
    Renderer::GetEntityIdFromPixels(mx, my, m_commonData->renderPass->GetSpecification().targetFramebuffer->GetPixelIdIndex(), pixeldData);
  }
} // namespace IKan
