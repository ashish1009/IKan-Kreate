//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Renderer.hpp"

namespace IKan
{
  // Renderer Utils --------------------------------------------------------------------------------------------------
  namespace RendererUtils
  {
#ifdef IK_DEBUG
    /// This function returns the Renderer API Type in const char string_view
    /// - Parameter rendererType: renderer API type
    std::string_view GetRendererTypeString(RendererType rendererType)
    {
      switch (rendererType)
      {
        case RendererType::OpenGL: return "OpenGL";
        case RendererType::Invalid:
        default:
          IK_ASSERT(false, "Invalid Renderer Type");
      }
    }
#endif
  } // namespace RendererUtils

  // Renderer Data ---------------------------------------------------------------------------------------------------
  /// This structure stores the renderer data
  struct RendererData
  {
    RendererType rendererType {RendererType::Invalid};
    Scope<RenderCommandQueue> commandQueue;

    /// This function destroys the renderer data
    void Shutdown()
    {
      rendererType = RendererType::Invalid;
      
      // Execute all commands before destory
      commandQueue->Execute();
      commandQueue.reset();
    }
  };
  static RendererData s_rendererData;

  // Renderer APIs ---------------------------------------------------------------------------------------------------
  void Renderer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Renderer, "Initializing the Renderers");
    
    // Check the Renderer API type
    if (s_rendererData.rendererType == RendererType::Invalid)
    {
      IK_LOG_CRITICAL(LogModule::Renderer, "Renderer API Type is not set or set as invalid."
                      "Call Renderer::SetCurrentRendererAPI(RendererType) before Renderer::Initialize() to set Renderer API type."
                      "'RendererType should not be RendererType::Invalid'");
      IK_ASSERT(false , "Renderer API type is not set")
    }
    
    // Create Render Command Queue before using any Renderer APIs
    s_rendererData.commandQueue = CreateScope<RenderCommandQueue>();
  }
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Renderer, "Shutting down the Renderers");
    
    // Destroy the Renderer Data
    // Note: To be destroyed in the end 
    s_rendererData.Shutdown();
  }
  
  // Renderer API ----------------------------------------------------------------------------------------------------
  void Renderer::SetCurrentRendererAPI(RendererType rendererType)
  {
    IK_PROFILE();
    IK_ASSERT(rendererType != RendererType::Invalid, "Invalid Renderer Type");
    IK_LOG_INFO(LogModule::Renderer, "Renderer Type is set to {0}", RendererUtils::GetRendererTypeString(rendererType));
    s_rendererData.rendererType = rendererType;
  }
  RendererType Renderer::GetCurrentRendererAPI()
  {
    return s_rendererData.rendererType;
  }
  
  // Render Command Queue --------------------------------------------------------------------------------------------
  void Renderer::WaitAndRender()
  {
    IK_PERFORMANCE("Renderer::WaitAndRender");
    IK_ASSERT(s_rendererData.commandQueue, "Render Command Queue is NULL");
    s_rendererData.commandQueue->Execute();
  }
  RenderCommandQueue* Renderer::GetRenderCommandQueue()
  {
    IK_ASSERT(s_rendererData.commandQueue, "Render Command Queue is NULL");
    return s_rendererData.commandQueue.get();
  }
} // namespace IKan
