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
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::SceneRenderer, "Creating Scene Renderer instance");
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
      
      m_commonData->highlightMaterial = Material::Create(CoreAssetPath("Shaders/MeshStencilShader.glsl"), "Mesh Stencil");
    }
  }
  
  SceneRenderer::~SceneRenderer()
  {
    IK_PROFILE();
    // Destroy the Render Pass for Renderer 2D
    if (m_commonData)
    {
      m_commonData.reset();
    }
    IK_LOG_TRACE(LogModule::SceneRenderer, "Destroying Scene Renderer instance");
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
  
  void SceneRenderer::SubmitPointLight(const PointLightData &pointLight)
  {
    m_commonData->pointLights[m_commonData->numLightStored++] = pointLight;
  }

  void SceneRenderer::BeginScene(const SceneCameraData& sceneCamera)
  {
    if (m_commonData->needResize)
    {
      // Resize the framebuffer
      m_commonData->renderPass->Resize(m_commonData->viewportWidth, m_commonData->viewportHeight);
    }
    m_commonData->sceneCamera = sceneCamera;
    
    // Reset all lights before scene begins
    m_commonData->numLightStored = 0;
    for (uint32_t i = 0; i < MAX_LIGHTS; i++)
    {
      m_commonData->pointLights[i].active = false;
    }
  }
  
  void SceneRenderer::SubmitMeshSource(Ref<MeshSource> mesh, const glm::mat4& transform, const Ref<Material>& oevrridenMaterial)
  {
    if (!mesh)
    {
      return;
    }
      
    MeshSourceDrawCommand dc;
    dc.staticMesh = mesh;
    dc.transform = transform;
    dc.oevrridenMaterial = oevrridenMaterial;
    
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
                                 const Ref<Material>& material, const glm::mat4& transform)
  {
    pipeline->Bind();
    material->Set("u_ViewProjection", m_commonData->sceneCamera.viewProjection);
    
    for (const SubMesh& submesh : submeshes)
    {
      material->Set("u_Transform", transform * submesh.transform);
      material->Bind();
      Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
      material->Unbind();
    } // for each submeshes
    pipeline->Unbind();
  }
  
  void SceneRenderer::GeometryPass()
  {
    for (const auto& dc : m_meshSourceDrawList)
    {
      auto material = dc.oevrridenMaterial ? dc.oevrridenMaterial : dc.staticMesh->GetBaseMaterial();
      
      // Submit the Light to PBR Shader
      {
        material->Set("u_LightData", m_commonData->pointLights.at(0));
      }
      
      RenderMesh(dc.staticMesh->GetSubMeshes(), dc.staticMesh->GetPipeline(), material, dc.transform);
    }
  }
  
  void SceneRenderer::CompositePass()
  {
    for (const auto& dc : m_selectedMeshSourceDrawList)
    {
      // Stencil Pass Mesh Render
      {
        Renderer::EnableStencilPass();

        // Hack to change size of mesh
        glm::vec3 p, r, s;
        Utils::Math::DecomposeTransform(dc.transform, p, r, s);
        float scaleFactor = Utils::Math::FloatClamp(0.003 * m_commonData->sceneCamera.distance, 0.005, 0.05);
        s += scaleFactor;
        auto tt = Utils::Math::GetTransformMatrix(p, r, s);

        RenderMesh(dc.staticMesh->GetSubMeshes(), dc.staticMesh->GetPipeline(), m_commonData->highlightMaterial, tt);
        
        Renderer::DisableStencilPass();
      }

      // Geometry pass Selected Mesh
      RenderMesh(dc.staticMesh->GetSubMeshes(), dc.staticMesh->GetPipeline(), dc.staticMesh->GetBaseMaterial(), dc.transform);
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
