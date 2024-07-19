//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 17/07/24.
//

#include "SceneRenderer.hpp"

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
  };
  
  // Scene Renderer Data ---------------------------------------------------------------------------------------------
  void SceneRendererData::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::SceneRenderer, "Initializing Scene renderer common data");
  }
  
  void SceneRendererData::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::SceneRenderer, "Shutting down Scene renderer common data");
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
  
  void SceneRenderer::BeginScene()
  {
    IK_PERFORMANCE("SceneRenderer::BeginScene");
  }
  
  void SceneRenderer::EndScene()
  {
    IK_PERFORMANCE("SceneRenderer::EndScene");
    m_geometryRenderPass->Bind();
    {
      Renderer::Clear({0.2f, 0.2f, 0.2f, 1.0f});
      
      // Debug Renderer callback
      {
        m_debugRenderer();
      }
    }
    m_geometryRenderPass->Unbind();
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
} // namespace IKan
