//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include "SceneRenderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  void SceneRenderer::Initialize()
  {
    s_colorMaterial = Material::Create(CoreAsset("Shaders/ColorShader.glsl"), "Color Shader");

    s_finalImageShader = ShaderFactory::Create(CoreAsset("Shaders/HdrBloomShader.glsl"));
    s_equiractangularToCubemapShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/EquirectangularToCubemapShader.glsl");
    s_irradianceShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/IrradianceShader.glsl");
    s_prefilterShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/PrefilterShader.glsl");
    s_skymapShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/SkymapShader.glsl");
  }
  
  void SceneRenderer::Shutdown()
  {
    s_colorMaterial.reset();
    s_finalImageShader.reset();
    s_equiractangularToCubemapShader.reset();
    s_irradianceShader.reset();
    s_prefilterShader.reset();
    s_skymapShader.reset();
  }
  
  SceneRenderer::SceneRenderer(const std::string& debugName)
  : m_debugName(debugName)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Creating Scene renderer for '{0}'", m_debugName);
    
    // Viewport Pass ----
    FrameBufferSpecification fbSpec;
    fbSpec.debugName = "Final Image Pass";
    fbSpec.attachments =
    {
      FrameBufferAttachments::TextureFormat::RGBA8,
      FrameBufferAttachments::TextureFormat::Depth24Stencil
    };
    m_viewportRenderPass = FrameBufferFactory::Create(fbSpec);
    
    // Geometry pass
    fbSpec.debugName = "Geometry Pass";
    fbSpec.attachments =
    {
      FrameBufferAttachments::TextureFormat::RGBA16F,
      FrameBufferAttachments::TextureFormat::RGBA16F,
      FrameBufferAttachments::TextureFormat::RGBA16F,
      FrameBufferAttachments::TextureFormat::R32I,
      FrameBufferAttachments::TextureFormat::Depth24Stencil,
    };
    m_geometryRenderPass = FrameBufferFactory::Create(fbSpec);
  }
  
  SceneRenderer::~SceneRenderer()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Destroying Scene renderer for '{0}'", m_debugName);
  }
  
  void SceneRenderer::BeginScene(const SceneRendererCamera& sceneCamera, const DirectionLight& directionLight)
  {
    s_sceneCamera = sceneCamera;
    s_directionLight = directionLight;
    IK_PERFORMANCE("SceneRenderer::BeginScene");
  }
  
  void SceneRenderer::EndScene()
  {
    IK_PERFORMANCE("SceneRenderer::EndScene");
    GeometryPass();

    FinalPass();
    
    // Clear draw list
    {
      m_meshDrawList.clear();
      m_selectedMeshDrawList.clear();
    }
  }
  
  void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
  {
    IK_PERFORMANCE("SceneRenderer::SetViewportSize");
    RETURN_IF(m_viewportWidth == width and m_viewportHeight == height);
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
  
  void SceneRenderer::SetDebugRenderer(const std::function<void()>& func)
  {
    m_debugRenderer = func;
  }

  void SceneRenderer::SubmitMesh(AssetHandle meshHandle, const glm::mat4& transform, Ref<MaterialTable> materilTable, float tilingFactor)
  {
    IK_PERFORMANCE("SceneRenderer::SubmitMesh");
    const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
    RETURN_IF(!mesh);
    
    m_meshDrawList.push_back({mesh, materilTable, tilingFactor, transform});
  }

  void SceneRenderer::SubmitSelectedMesh(AssetHandle meshHandle, const glm::mat4& transform, Ref<MaterialTable> materilTable, float tilingFactor)
  {
    IK_PERFORMANCE("SceneRenderer::SubmitSelectedMesh");
    const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
    RETURN_IF(!mesh);
    
    m_selectedMeshDrawList.push_back({mesh, materilTable, tilingFactor, transform});
  }

  void SceneRenderer::RenderMeshGeometry(Ref<Mesh> mesh, const glm::mat4& transform, float tilingFactor, Ref<Material> material)
  {
    RETURN_IF(!material);
    
    material->Set("u_TilingFactor", tilingFactor);
    material->Set("u_ViewProjection", s_sceneCamera.camera.GetUnReversedProjectionMatrix() * s_sceneCamera.viewMatrix);
    material->Set("u_CameraPosition", s_sceneCamera.position);
    material->Set("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));
    material->Set("u_DirectionLight", s_directionLight);

    RenderSubmesh(mesh, transform, material);
  }
  void SceneRenderer::RenderSubmesh(Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material)
  {
    mesh->GetPipeline()->Bind();
    for (const SubMesh& submesh : mesh->GetSubMeshes())
    {
      material->Set("u_Transform", transform * submesh.transform);
      material->Bind();
      Renderer::DrawIndexedBaseVertex(submesh.indexCount, (void*)(sizeof(uint32_t) * submesh.baseIndex), submesh.baseVertex);
    } // for each submeshes
    material->Unbind();
    mesh->GetPipeline()->Unbind();
  }
  
  void SceneRenderer::GeometryPass()
  {
    IK_PERFORMANCE("SceneRenderer::GeometryPass");
    m_geometryRenderPass->Bind();
    {
      Renderer::Clear({0.23f, 0.234f, 0.2345f, 1.0f});
      
      // Geometry pass
      {
        for (const auto& meshData : m_meshDrawList)
        {
          // TODO: Get index from somewhere
          if (meshData.materilTable->HasMaterial(0))
          {
            RenderMeshGeometry(meshData.mesh, meshData.transform, meshData.tilingFactor, meshData.materilTable->GetMaterial(0)->GetMaterial());
          }
        }
      }
      
      // Composit pass
      {
        CompositePass();
      }
      
      // Debug Renderer
      {
        // Debuig renderer callback
        m_debugRenderer();
      }
    }
    m_geometryRenderPass->Unbind();
  }
  
  void SceneRenderer::CompositePass()
  {
    IK_PERFORMANCE("SceneRenderer::CompositePass");
    for (const auto& selectedMeshData : m_selectedMeshDrawList)
    {
      // Render Outline Mesh ----------------------------------------------------------------------
      Renderer::EnableStencilPass();
      
      static glm::vec4 selectedMeshColor = {0.5f, 0.3f, 0.2f, 0.7f};
      s_colorMaterial->Set("u_ObjectColor", selectedMeshColor);
      
      // Hack to change size of mesh
      static glm::vec3 p, r, s;
      Utils::Math::DecomposeTransform(selectedMeshData.transform, p, r, s);
      
      if (s.x < 1.0f) s.x += (s.x * 0.1f); else s.x += 0.05f;
      if (s.y < 1.0f) s.y += (s.y * 0.1f); else s.y += 0.05f;
      if (s.z < 1.0f) s.z += (s.z * 0.1f); else s.z += 0.05f;
      
      auto modTransform = Utils::Math::GetTransformMatrix(p, r, s);
      
      // Render Outline
      s_colorMaterial->Set("u_ViewProjection", s_sceneCamera.camera.GetUnReversedProjectionMatrix() * s_sceneCamera.viewMatrix);
      RenderSubmesh(selectedMeshData.mesh, modTransform, s_colorMaterial);
      
      Renderer::DisableStencilPass();
      
      // Render Original Mesh
      // TODO: Get index from somewhere
      if (selectedMeshData.materilTable->HasMaterial(0))
      {
        RenderMeshGeometry(selectedMeshData.mesh, selectedMeshData.transform, selectedMeshData.tilingFactor, selectedMeshData.materilTable->GetMaterial(0)->GetMaterial());
      }
    }
  }
  
  void SceneRenderer::BloomPass()
  {
    IK_PERFORMANCE("SceneRenderer::BloomPass");
  }
  
  void SceneRenderer::FinalPass()
  {
    IK_PERFORMANCE("SceneRenderer::FinalPass");
    // Render Scene in main Viewport ----------------------------------------------------------
    m_viewportRenderPass->Bind();
    Renderer::ClearBits();
    
    s_finalImageShader->Bind();
    Renderer2D::DrawFullscreenQuad(m_geometryRenderPass->GetColorAttachments().at(0), 0, true);
    s_finalImageShader->Unbind();
    
    m_viewportRenderPass->Unbind();
  }

  Ref<Texture> SceneRenderer::GetFinalImage() const
  {
    return m_viewportRenderPass->GetColorAttachments().at(0);
  }
  
  void SceneRenderer::SubmitSkyboxImage(const AssetHandle& skyboxAsset)
  {
    IK_PROFILE();
    ImageSpecificaion spec;
    spec.filePath = AssetManager::GetFileSystemPath(AssetManager::GetMetadata(skyboxAsset));
    spec.wrap = TextureWrap::ClampEdge;
    spec.filter = TextureFilter::Linear;
    s_envTexture = TextureFactory::Create(spec);
    SubmitSkyboxImageImp();
  }
  
  void SceneRenderer::SubmitSkyboxImage(const std::string& skyboxTexturePath)
  {
    IK_PROFILE();
    ImageSpecificaion spec;
    spec.filePath = skyboxTexturePath;
    spec.wrap = TextureWrap::ClampEdge;
    spec.filter = TextureFilter::Linear;
    s_envTexture = TextureFactory::Create(spec);
    SubmitSkyboxImageImp();
  }
  
  void SceneRenderer::SubmitSkyboxImageImp()
  {
    IK_PROFILE();
  }
} // namespace IKan
