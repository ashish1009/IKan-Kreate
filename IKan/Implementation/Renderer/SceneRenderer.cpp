//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#include "SceneRenderer.hpp"
#include "Assets/AssetManager.hpp"

namespace IKan
{
  /// This structure stores the common scene renderer Data
  struct SceneRendererData
  {
    // Member functions -------------------------------------------
    /// This function initializes the common data for scene renderer
    static void Initialize();
    /// This function shuts down the common data for scene renderer
    static void Shutdown();
    
    // Member viariabls -------------------------------------------
    inline static uint32_t s_numRenderers {0};
    inline static SceneRendererCamera s_sceneCamera;
    inline static Ref<Material> s_defaultMaterial;
  };
  
  // Scene Renderer Data ---------------------------------------------------------------------------------------------
  void SceneRendererData::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Initializing Scene renderer common data");

    // Set default material
    s_defaultMaterial = Material::Create(ShaderLibrary::Get(CoreAsset("Shaders/PBR_StaticShader.glsl")));

    auto& mat = s_defaultMaterial->Get<MaterialProperty>("u_Material");
    mat.color = {0.8f, 0.8f, 0.8f};
    mat.metallic = 0.5f;
    mat.roughness = 0.5f;
    mat.depthScale = 0.001f;
    s_defaultMaterial->Set<MaterialProperty>("u_Material", mat);
  }
  
  void SceneRendererData::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::SceneRenderer, "Shutting down Scene renderer common data");
    
    s_defaultMaterial.reset();
  }
  
  // Scene Renderer APIs ---------------------------------------------------------------------------------------------
  SceneRenderer::SceneRenderer(std::string_view debugName)
  : m_debugName(debugName)
  {
    IK_PROFILE();
    
    // Initilize the common data if not initialized yet
    if (0 == SceneRendererData::s_numRenderers)
    {
      SceneRendererData::Initialize();
    }
    
    IK_LOG_INFO(LogModule::SceneRenderer, "Creating Scene renderer for '{0}'", m_debugName);
    IK_LOG_TRACE(LogModule::SceneRenderer, "Total renderers created : {0}", ++SceneRendererData::s_numRenderers);
    
    // Geometry pass
    FrameBufferSpecification fbSpec;
    fbSpec.debugName = std::string("Geometry Pass : ") + std::string(debugName);
    fbSpec.attachments =
    {
      FrameBufferAttachments::TextureFormat::RGBA8,
      FrameBufferAttachments::TextureFormat::Depth24Stencil
    };
    m_geometryRenderPass = FrameBufferFactory::Create(fbSpec);
  }
  
  SceneRenderer::~SceneRenderer()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::SceneRenderer, "Destroying Scene renderer for '{0}'", m_debugName);
    IK_LOG_TRACE(LogModule::SceneRenderer, "Remaining {0} renderers", --SceneRendererData::s_numRenderers);
    
    // Destroy the common data if all renderers are destroyed
    if (0 == SceneRendererData::s_numRenderers)
    {
      SceneRendererData::Shutdown();
    }
  }
  
  void SceneRenderer::BeginScene(const SceneRendererCamera& sceneCamera)
  {
    IK_PERFORMANCE("SceneRenderer::BeginScene");
    SceneRendererData::s_sceneCamera = sceneCamera;
  }
  
  void SceneRenderer::EndScene()
  {
    IK_PERFORMANCE("SceneRenderer::EndScene");
    m_geometryRenderPass->Bind();
    {
      Renderer::Clear({0.2f, 0.2f, 0.2f, 1.0f});
      
      // Geometry pass
      {
        for (const auto& meshData : m_meshDrawList)
        {
          // TODO: Get index from somewhere
          if (meshData.materilTable->HasMaterial(0))
          {
            RenderMeshGeometry(meshData.mesh, meshData.transform, meshData.tilingFactor, meshData.materilTable->GetMaterial(0)->GetMaterial());
          }
          else
          {
            RenderMeshGeometry(meshData.mesh, meshData.transform, meshData.tilingFactor, SceneRendererData::s_defaultMaterial);
          }
        }
      }

      // Debug Renderer callback
      {
        m_debugRenderer();
      }
    }
    m_geometryRenderPass->Unbind();
    
    // Clear draw list
    {
      m_meshDrawList.clear();
    }
  }
  
  void SceneRenderer::RenderMeshGeometry(Ref<Mesh> mesh, const glm::mat4& transform, float tilingFactor, Ref<Material> material)
  {
    IK_PERFORMANCE("SceneRenderer::RenderMeshGeometry");
    if (!material)
    {
      return;
    }
        
    material->Set("u_TilingFactor", tilingFactor);
    material->Set("u_ViewProjection", SceneRendererData::s_sceneCamera.camera.GetUnReversedProjectionMatrix() * SceneRendererData::s_sceneCamera.viewMatrix);
    material->Set("u_CameraPosition", SceneRendererData::s_sceneCamera.position);
    material->Set("u_NormalMatrix", glm::transpose(glm::inverse(glm::mat3(transform))));
    
    RenderSubmesh(mesh, transform, material);
  }
  void SceneRenderer::RenderSubmesh(Ref<Mesh> mesh, const glm::mat4& transform, Ref<Material> material)
  {
    IK_PERFORMANCE("SceneRenderer::RenderSubmesh");
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
  
  void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
  {
    IK_PROFILE()
    IK_PERFORMANCE("SceneRenderer::SetViewportSize");
    if (0 == width or 0 == height or (m_viewportWidth == width and m_viewportHeight == height))
    {
      return;
    }
    
    m_viewportWidth = width;
    m_viewportHeight = height;
    
    m_geometryRenderPass->Resize(m_viewportWidth, m_viewportHeight);
  }
  
  void SceneRenderer::SetDebugRenderer(const std::function<void()>& func)
  {
    m_debugRenderer = func;
  }

  Ref<Texture> SceneRenderer::GetFinalImage() const
  {
    return m_geometryRenderPass->GetColorAttachments().at(0);
  }
  
  void SceneRenderer::SubmitMesh(AssetHandle meshHandle, const glm::mat4& transform, Ref<MaterialTable> materilTable, float tilingFactor)
  {
    IK_PERFORMANCE("SceneRenderer::SubmitMesh");
    
    const auto& mesh = AssetManager::GetAsset<Mesh>(meshHandle);
    if (!mesh)
    {
      return;
    }
    
    m_meshDrawList.push_back({mesh, materilTable, tilingFactor, transform});
  }

} // namespace IKan
