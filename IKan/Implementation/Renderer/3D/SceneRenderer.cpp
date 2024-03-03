//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include "SceneRenderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/Renderer3D.hpp"
#include "Asset/AssetManager.hpp"

namespace IKan
{
  // Set up projection and view matrices for capturing data onto the 6 cubemap face directions
  static glm::mat4 CaptureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
  static glm::mat4 CaptureViews[] =
  {
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
  };

  void SceneRenderer::Initialize()
  {
    IK_PROFILE();
    s_colorMaterial = Material::Create(CoreAsset("Shaders/ColorShader.glsl"), "Color Shader");

    s_finalImageShader = ShaderFactory::Create(CoreAsset("Shaders/HdrBloomShader.glsl"));
    s_equiractangularToCubemapShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/EquirectangularToCubemapShader.glsl");
    s_irradianceShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/IrradianceShader.glsl");
    s_prefilterShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/PrefilterShader.glsl");
    s_skymapShader = ShaderFactory::Create("../../../IKan/Assets/Shaders/SkymapShader.glsl");
  }
  
  void SceneRenderer::Shutdown()
  {
    IK_PROFILE();
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
    
    if (s_isIBL and s_envTexture)
    {
      material->Set("u_IBLToggle", 1.0f);
      material->Set("u_IrradianceMap", s_irradianceMap);
      material->Set("u_PrefilterMap", s_prefilterMap);
    }
    else
    {
      material->Set("u_IBLToggle", 0.0f);
    }

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
      
      // Skybox Renderer
      {
        // Render skybox (render as last to prevent overdraw)
        if (s_isIBL and s_envTexture)
        {
          s_skymapShader->Bind();
          s_skymapShader->SetUniformMat4("u_Projection", s_sceneCamera.camera.GetProjectionMatrix());
          s_skymapShader->SetUniformMat4("u_RotateView", glm::mat4(glm::mat3(s_sceneCamera.viewMatrix)));          
          s_showIrradiance ? s_irradianceMap->Bind() : s_envCubemap->Bind();
          
          Renderer::DepthFunc(GlDepthFunc::LEqual);
          Renderer3D::DrawFullscreenCube();
          Renderer::DepthFunc(GlDepthFunc::Less);
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
    if (const AssetMetadata& assetMetadata = AssetManager::GetMetadata(skyboxAsset); assetMetadata.IsValid())
    {
      spec.filePath = AssetManager::GetFileSystemPath(assetMetadata);
      spec.wrap = TextureWrap::ClampEdge;
      spec.filter = TextureFilter::Linear;
      s_envTexture = TextureFactory::Create(spec);
      SubmitSkyboxImageImp();
    }
    else
    {
      s_isIBL = false;
    }
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
    
    s_isIBL = true;
    
    // 1. Setup Capture framebuffer
    FrameBufferSpecification fbSpec;
    fbSpec.width = 512;
    fbSpec.height = 512;
    Ref<FrameBuffer> captureFB = FrameBufferFactory::Create(fbSpec);

    // 2. Setup cubemap to render to and attach to framebuffer
    Texture2DSpecification spec;
    spec.width = 512;
    spec.height = 512;
    spec.internalFormat = TextureFormat::RGBA16F;
    spec.dataFormat = TextureFormat::RGB;
    spec.wrap = TextureWrap::ClampEdge;
    spec.filter = TextureFilter::Linear;
    spec.type = TextureType::TextureCubemap;
    s_envCubemap = TextureFactory::Create(spec);

    // 3. Convert HDR equirectangular environment map to cubemap equivalent
    s_equiractangularToCubemapShader->Bind();
    s_equiractangularToCubemapShader->SetUniformMat4("u_Projection", CaptureProjection);
    
    Renderer::SetViewport(512, 512);
    captureFB->Bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
      s_equiractangularToCubemapShader->SetUniformMat4("u_View", CaptureViews[i]);
      s_envCubemap->AttachToFramebuffer(TextureAttachment::Color, 0, i);
      Renderer::ClearBits();
      s_envTexture->Bind();
      Renderer3D::DrawFullscreenCube();
    }
    captureFB->Unbind();
    
    // 4. Let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    s_envCubemap->Bind();
    
    // 5. Create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    spec.width = 32;
    spec.height =32;
    s_irradianceMap = TextureFactory::Create(spec);
    
    captureFB->Bind();
    
    // 6. Solve diffuse integral by convolution to create an irradiance (cube)map.
    s_irradianceShader->Bind();
    s_irradianceShader->SetUniformMat4("u_Projection", CaptureProjection);
    
    captureFB->Bind();
    Renderer::SetViewport(32, 32);
    for (unsigned int i = 0; i < 6; ++i)
    {
      s_irradianceShader->SetUniformMat4("u_View", CaptureViews[i]);
      s_irradianceMap->AttachToFramebuffer(TextureAttachment::Color, 0, i);
      Renderer::ClearBits();
      s_envCubemap->Bind();
      Renderer3D::DrawFullscreenCube();
    }
    captureFB->Unbind();
    
    // 7. Create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    spec.width = 128;
    spec.height =128;
    spec.filter =TextureFilter::LinearMipmapLinear;
    s_prefilterMap = TextureFactory::Create(spec);
    
    // 8. Run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    s_prefilterShader->Bind();
    s_prefilterShader->SetUniformMat4("u_Projection", CaptureProjection);
    
    captureFB->Bind();
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
      // reisze framebuffer according to mip-level size.
      unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
      unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
      Renderer::SetViewport(mipWidth, mipHeight);
      
      float roughness = (float)mip / (float)(maxMipLevels - 1);
      s_prefilterShader->SetUniformFloat1("u_Roughness", roughness);
      for (unsigned int i = 0; i < 6; ++i)
      {
        s_prefilterShader->SetUniformMat4("u_View", CaptureViews[i]);
        s_prefilterMap->AttachToFramebuffer(TextureAttachment::Color, 0, i, mip);
        Renderer::ClearBits();
        s_envCubemap->Bind();
        Renderer3D::DrawFullscreenCube();
      }
    }
    captureFB->Unbind();
  }
  
  void SceneRenderer::SetIBLFlag(bool flag)
  {
    s_isIBL = flag;
  }
  void SceneRenderer::SetIrradianceFlag(bool flag)
  {
    s_showIrradiance = flag;
  }
} // namespace IKan
