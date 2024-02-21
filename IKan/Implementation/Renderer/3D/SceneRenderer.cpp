//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 21/02/24.
//

#include "SceneRenderer.hpp"

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
  }
  
  void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
  {
    IK_PERFORMANCE("SceneRenderer::SetViewportSize");
    RETURN_IF(m_viewportWidth == width and m_viewportHeight == height);
    m_viewportWidth = width;
    m_viewportHeight = height;
  }
} // namespace IKan
