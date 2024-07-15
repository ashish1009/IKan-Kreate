//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "Renderer.hpp"

#include "Renderer/Renderer2D.hpp"
#include "Renderer/Graphics/RendererAPI.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/UI/Font.hpp"

namespace IKan
{
  // Renderer Utils --------------------------------------------------------------------------------------------------
  namespace RendererUtils
  {
#ifdef IK_DEBUG
    /// This function returns the Renderer API Type in const char string_view
    /// - Parameter rendererType: renderer API type
    static std::string_view GetRendererTypeString(RendererType rendererType)
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
    Scope<RendererAPI> rendererAPI;

    /// This function destroys the renderer data
    void Shutdown()
    {
      rendererType = RendererType::Invalid;
      rendererAPI.reset();

      // Execute all commands before destory
      commandQueue->Execute();
      commandQueue.reset();
    }
  };
  static RendererData s_rendererData;

  // Renderer Capabilities -------------------------------------------------------------------------------------------
  RendererCapabilities& RendererCapabilities::Get()
  {
    static RendererCapabilities capabilities;
    return capabilities;
  }

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
    
    // Create Renderer API instance
    s_rendererData.rendererAPI = RendererAPIFactory::Create();
    
    // Inittialze the Renderers
    Renderer2D::Initialize();
    Font::Initialize();
  }
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Renderer, "Shutting down the Renderers");
    
    // Shut down the renderers
    Renderer2D::Shutdown();
    ShaderLibrary::Clear();
    Font::Shutdown();
    FontLibrary::Clear();

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
  
  // Renderer Controller ---------------------------------------------------------------------------------------------
  void Renderer::Clear(const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer::Clear");
    s_rendererData.rendererAPI->SetClearColor(color);
    s_rendererData.rendererAPI->ClearBits();
  }
  void Renderer::ClearBits()
  {
    IK_PERFORMANCE("Renderer::ClearBits");
    s_rendererData.rendererAPI->ClearBits();
  }
  void Renderer::ClearColorBits()
  {
    IK_PERFORMANCE("Renderer::ClearColorBits");
    s_rendererData.rendererAPI->ClearColorBits();
  }
  void Renderer::ClearDepthBits()
  {
    IK_PERFORMANCE("Renderer::ClearDepthBits");
    s_rendererData.rendererAPI->ClearDepthBits();
  }
  
  // Draw Calls -----------------------------------------------------------------------------------------------------
  void Renderer::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    IK_PERFORMANCE("Renderer::DrawIndexed");
    s_rendererData.rendererAPI->DrawIndexed(pipeline, count);
  }
  void Renderer::DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount)
  {
    IK_PERFORMANCE("Renderer::DrawLines");
    s_rendererData.rendererAPI->DrawLines(pipeline, vertexCount);
  }
  void Renderer::DrawArrays(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    IK_PERFORMANCE("Renderer::DrawArrays");
    s_rendererData.rendererAPI->DrawArrays(pipeline, count);
  }
  void Renderer::DrawQuad(const Ref<Pipeline>& pipeline)
  {
    IK_PERFORMANCE("Renderer::DrawQuad");
    s_rendererData.rendererAPI->DrawQuad(pipeline);
  }
  void Renderer::DrawIndexedBaseVertex(uint32_t indexCount, void* indicesData, uint32_t baseVertex)
  {
    IK_PERFORMANCE("Renderer::DrawIndexedBaseVertex");
    s_rendererData.rendererAPI->DrawIndexedBaseVertex(indexCount, indicesData, baseVertex);
  }
} // namespace IKan
