//
//  Renderer.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Renderer.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/TextRenderer.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/RendererAPI.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
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
    
    TextRenderer::Initialize();
    Font::Initialize();
  }
  
  void Renderer::Shutdown()
  {
    IK_PROFILE();
    
    // Shutdown All renderer
    Font::Shutdown();
    TextRenderer::Shutdown();
    
    // Reset Libraries
    Shader::ResetLibrary();
    Image::ResetLibrary();

    s_rendererData.reset();
  }
  
  // Renderer Controller -------------------------------------------------------------------------------------------
  void Renderer::Clear(const glm::vec4& color)
  {
    s_rendererData->rendererApiInstance->SetClearColor(color);
    s_rendererData->rendererApiInstance->ClearBits();
  }
  void Renderer::SetClearColor(const glm::vec4& color)
  {
    s_rendererData->rendererApiInstance->SetClearColor(color);
  }
  void Renderer::ClearBits()
  {
    s_rendererData->rendererApiInstance->ClearBits();
  }
  void Renderer::ClearDepthBit()
  {
    s_rendererData->rendererApiInstance->ClearDepthBit();
  }
  void Renderer::ClearColorBit()
  {
    s_rendererData->rendererApiInstance->ClearColorBit();
  }
  void Renderer::ClearStencilBit()
  {
    s_rendererData->rendererApiInstance->ClearStencilBit();
  }
  void Renderer::Depth(bool state)
  {
    s_rendererData->rendererApiInstance->Depth(state);
  }
  void Renderer::Blend(bool state)
  {
    s_rendererData->rendererApiInstance->Blend(state);
  }
  void Renderer::MultiSample(bool state)
  {
    s_rendererData->rendererApiInstance->MultiSample(state);
  }
  void Renderer::SetViewport(uint32_t width, uint32_t height)
  {
    s_rendererData->rendererApiInstance->SetViewport(width, height);
  }
  void Renderer::GetEntityIdFromPixels(int32_t mx, int32_t my, uint32_t pixelIDIndex, int32_t& pixelData)
  {
    s_rendererData->rendererApiInstance->GetEntityIdFromPixels(mx, my, pixelIDIndex, pixelData);
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
  
  // Draw Wrappers ------------------------------------------------------------------------------------------------
  void Renderer::DrawAABB(Ref<MeshSource> mesh, const glm::mat4& transform, const glm::vec4& color)
  {
    auto& submeshes = mesh->GetSubMeshes();
    for (const auto& submesh : submeshes)
    {
      auto& aabb = submesh.boundingBox;
      auto aabbTransform = transform * submesh.transform;
      DrawAABB(aabb, aabbTransform, color);
    }
  }
  
  void Renderer::DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color)
  {
    glm::vec4 min = { aabb.min.x, aabb.min.y, aabb.min.z, 1.0f };
    glm::vec4 max = { aabb.max.x, aabb.max.y, aabb.max.z, 1.0f };
    
    glm::vec4 corners[8] =
    {
      transform * glm::vec4 { aabb.min.x, aabb.min.y, aabb.max.z, 1.0f },
      transform * glm::vec4 { aabb.min.x, aabb.max.y, aabb.max.z, 1.0f },
      transform * glm::vec4 { aabb.max.x, aabb.max.y, aabb.max.z, 1.0f },
      transform * glm::vec4 { aabb.max.x, aabb.min.y, aabb.max.z, 1.0f },
      
      transform * glm::vec4 { aabb.min.x, aabb.min.y, aabb.min.z, 1.0f },
      transform * glm::vec4 { aabb.min.x, aabb.max.y, aabb.min.z, 1.0f },
      transform * glm::vec4 { aabb.max.x, aabb.max.y, aabb.min.z, 1.0f },
      transform * glm::vec4 { aabb.max.x, aabb.min.y, aabb.min.z, 1.0f }
    };
    
    for (uint32_t i = 0; i < 4; i++)
    {
      Renderer2D::DrawLine(corners[i], corners[(i + 1) % 4], color);
    }
    
    for (uint32_t i = 0; i < 4; i++)
    {
      Renderer2D::DrawLine(corners[i + 4], corners[((i + 1) % 4) + 4], color);
    }
    
    for (uint32_t i = 0; i < 4; i++)
    {
      Renderer2D::DrawLine(corners[i], corners[i + 4], color);
    }
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

  void Renderer::DrawQuad(const Ref<Pipeline>& pipeline)
  {
    s_rendererData->rendererApiInstance->DrawQuad(pipeline);
  }
  
  void Renderer::DrawIndexedBaseVertex(uint32_t indexCount, void* indicesData, uint32_t baseVertex)
  {
    s_rendererData->rendererApiInstance->DrawIndexedBaseVertex(indexCount, indicesData, baseVertex);
  }

  // Renderer Capabilities -------------------------------------------------------------------------------------------
  Renderer::Capabilities& Renderer::Capabilities::Get()
  {
    static Capabilities capabilities;
    return capabilities;
  }
} // namespace IKan
