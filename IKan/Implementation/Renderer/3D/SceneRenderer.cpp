//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include "SceneRenderer.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  SceneRenderer::SceneRenderer(const std::string& debugName)
  : m_debugName(debugName)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Creating Scene renderer for '{0}'", m_debugName);
    
    // Viewport Pass ----
    FrameBufferSpecification fbSpec;
    fbSpec.debugName = "Final Pass";
    fbSpec.attachments =
    {
      FrameBufferAttachments::TextureFormat::RGBA8,
      FrameBufferAttachments::TextureFormat::Depth24Stencil
    };
    m_viewportRenderPass = FrameBufferFactory::Create(fbSpec);
  }
  
  SceneRenderer::~SceneRenderer()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Destroying Scene renderer for '{0}'", m_debugName);
  }
  
  void SceneRenderer::BeginScene(const SceneRendererCamera& sceneCamera)
  {
    IK_PERFORMANCE("SceneRenderer::BeginScene");
  }
  
  void SceneRenderer::EndScene()
  {
    IK_PERFORMANCE("SceneRenderer::EndScene");
    
    m_viewportRenderPass->Bind();
    {
      Renderer::Clear({0.23f, 0.234f, 0.2345f, 1.0f});
      
      for (const auto& meshData : m_meshDrawList)
      {
        RenderMeshGeometry(meshData.mesh, meshData.transform);
      }
    }
    m_viewportRenderPass->Unbind();
    
    // Clear draw list
    {
      m_meshDrawList.clear();
    }
  }
  
  void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
  {
    IK_PERFORMANCE("SceneRenderer::SetViewportSize");
    RETURN_IF(m_viewportWidth == width and m_viewportHeight == height);
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
  
  void SceneRenderer::SubmitMesh(AssetHandle meshHandle, const glm::mat4& transform)
  {
    IK_PERFORMANCE("SceneRenderer::SubmitMesh");
    const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
    RETURN_IF(!mesh);
    m_meshDrawList.push_back({mesh, transform});
  }
  
  void SceneRenderer::RenderMeshGeometry(Ref<Mesh> mesh, const glm::mat4& transform)
  {
    RenderSubmesh(mesh, transform);
  }
  void SceneRenderer::RenderSubmesh(Ref<Mesh> mesh, const glm::mat4& transform)
  {
    mesh->GetPipeline()->Bind();
    for (const SubMesh& submesh : mesh->GetSubMeshes())
    {
      Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
    } // for each submeshes
    mesh->GetPipeline()->Unbind();
  }

  Ref<Texture> SceneRenderer::GetFinalImage() const
  {
    return m_viewportRenderPass->GetColorAttachments().at(0);
  }
} // namespace IKan
