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
} // namespace IKan
