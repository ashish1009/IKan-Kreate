//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Renderer.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/Graphics/RendererAPI.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Pipeline.hpp"

namespace IKan
{
  namespace RendererUtils
  {
    std::string GetRendererApiName(Renderer::Api api)
    {
      switch (api) {
        case Renderer::Api::OpenGl: return "Open GL";
        case Renderer::Api::None:
        default:
          IK_ASSERT(false, "Invalid Renderer API");
      }
    }
  } // namespace RendererUtils
  
  struct RendererData
  {
    Renderer::Api api = Renderer::Api::None;
    Scope<RendererAPI> rendererApiInstance;

    RendererData(Renderer::Api newApi)
    : api(newApi)
    {
      IK_ASSERT(api != Renderer::Api::None, "Invalid Renderer API");
      IK_LOG_TRACE(LogModule::Renderer, "Creating Renderer Data as {0} ", RendererUtils::GetRendererApiName(api));
    }
    ~RendererData()
    {
      rendererApiInstance.reset();
      IK_LOG_WARN(LogModule::Renderer, "Destroying Renderer Data as {0} ", RendererUtils::GetRendererApiName(api));
    }
  };
  static Scope<RendererData> s_rendererData;
  
  // Fundamentals ---------------------------------------------------------------------------------------------------
  void Renderer::CreateRendererData(Api api)
  {
    s_rendererData = CreateScope<RendererData>(api);
  }
  
  void Renderer::Initialize()
  {
    IK_PROFILE();
    s_rendererData->rendererApiInstance = RendererAPI::Create();
  }
  
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    
    // Reset Shader Library
    Shader::ResetLibrary();

    s_rendererData.reset();
  }
  
  // Renderer Stats API ---------------------------------------------------------------------------------------------
  void Renderer::ResetStatsEachFrame()
  {
    RendererStatistics::Get().ResetEachFrame();
  }
  void Renderer::ResetStats()
  {
    RendererStatistics::Get().ResetAll();
  }
  
  // Getters ---------------------------------------------------------------------------------------------------------
  Renderer::Api Renderer::GetApi()
  {
    IK_ASSERT(s_rendererData, "Renderer Data didnt initialised");
    return s_rendererData->api;
  }

  // Draw Calls -----------------------------------------------------------------------------------------------------
  void Renderer::DrawIndexed(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    s_rendererData->rendererApiInstance->DrawIndexed(pipeline, count);
  }
  
  void Renderer::DrawLines(const Ref<Pipeline>& pipeline, uint32_t vertexCount)
  {
    s_rendererData->rendererApiInstance->DrawLines(pipeline, vertexCount);
  }
  
  void Renderer::DrawArrays(const Ref<Pipeline>& pipeline, uint32_t count)
  {
    s_rendererData->rendererApiInstance->DrawArrays(pipeline, count);
  }
  
  // Renderer Capabilities -------------------------------------------------------------------------------------------
  Renderer::Capabilities& Renderer::Capabilities::Get()
  {
    static Capabilities capabilities;
    return capabilities;
  }
} // namespace IKan
