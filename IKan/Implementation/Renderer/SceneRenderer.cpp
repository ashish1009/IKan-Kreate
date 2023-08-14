//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#include "SceneRenderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  SceneRenderer::MeshKey::MeshKey()
  {
    
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
    
    // Create Pipeline specification
    Pipeline::Specification geomatryPipelineSpec;
    geomatryPipelineSpec.debugName = "PBR-Static";
    geomatryPipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/PBR_StaticShader.glsl"));
    geomatryPipelineSpec.vertexLayout =
    {
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Normal",    ShaderDataType::Float3 },
      { "a_Tangent",   ShaderDataType::Float3 },
      { "a_Bitangent", ShaderDataType::Float3 },
      { "a_TexCoord",  ShaderDataType::Float2 },
    };
    
    geomatryPipelineSpec.instanceLayout =
    {
      { "a_MRow0",     ShaderDataType::Float4 },
      { "a_MRow1",     ShaderDataType::Float4 },
      { "a_MRow2",     ShaderDataType::Float4 },
    };
    
    // Create the Pipeline instnace for full screen quad
    m_geometryPipeline = Pipeline::Create(geomatryPipelineSpec);
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
    
    Renderer2D::BeginBatch(camViewProjMat);
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
    for (auto& [mk, dc] : m_meshSourceDrawList)
    {
      
    }
  }

  void SceneRenderer::SubmitMeshSource(Ref<MeshSource> meshSource)
  {
    
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
