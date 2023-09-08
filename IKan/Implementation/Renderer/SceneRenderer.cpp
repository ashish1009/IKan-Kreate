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

  void SceneRenderer::BeginScene(const glm::mat4& camViewProjMat)
  {
    if (m_commonData->needResize)
    {
      // Resize the framebuffer
      m_commonData->renderPass->Resize(m_commonData->viewportWidth, m_commonData->viewportHeight);
    }
    m_commonData->camViewProjection = camViewProjMat;
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
  
  void SceneRenderer::GeometryPass()
  {
    for (const auto& dc : m_meshSourceDrawList)
    {
      // draw floor as normal, but don't write the floor to the stencil buffer, we only care about the containers.
      // We set its mask to 0x00 to not write to the stencil buffer.
      glStencilMask(0x00);
      
      auto pipeline = dc.staticMesh->GetPipeline();
      pipeline->Bind();
      
      auto shader = pipeline->GetSpecification().shader;
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", m_commonData->camViewProjection);
      
      for (const SubMesh& submesh : dc.staticMesh->GetSubMeshes())
      {
        shader->SetUniformMat4("u_Transform", dc.transform * submesh.transform);
        Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
      } // for each submeshes
    }
  }
  
  void SceneRenderer::CompositePass()
  {
    for (const auto& dc : m_selectedMeshSourceDrawList)
    {
      {
        // 2nd. render pass: now draw slightly scaled versions of the objects, this time disabling stencil writing.
        // Because the stencil buffer is now filled with several 1s. The parts of the buffer that are 1 are not drawn, thus only drawing
        // the objects' size differences, making it look like borders.
        // -----------------------------------------------------------------------------------------------------------------------------
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        auto pipeline = dc.staticMesh->GetPipeline();
        pipeline->Bind();

        auto shader = m_commonData->stencilShader;
        shader->Bind();
        shader->SetUniformMat4("u_ViewProjection", m_commonData->camViewProjection);
        
        glm::vec3 p, r, s;
        Utils::Math::DecomposeTransform(dc.transform, p, r, s);
        s += 0.1;
        auto tt = Utils::Math::GetTransformMatrix(p, r, s);
        
        for (const SubMesh& submesh : dc.staticMesh->GetSubMeshes())
        {
          shader->SetUniformMat4("u_Transform", tt * submesh.transform);
          Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
        } // for each submeshes
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);
      }

      {
        // 1st. render pass, draw objects as normal, writing to the stencil buffer
        // --------------------------------------------------------------------
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        
        auto pipeline = dc.staticMesh->GetPipeline();
        pipeline->Bind();
        
        auto shader = pipeline->GetSpecification().shader;
        shader->Bind();
        shader->SetUniformMat4("u_ViewProjection", m_commonData->camViewProjection);
        
        for (const SubMesh& submesh : dc.staticMesh->GetSubMeshes())
        {
          shader->SetUniformMat4("u_Transform", dc.transform * submesh.transform);
          Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
        } // for each submeshes
      }
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
