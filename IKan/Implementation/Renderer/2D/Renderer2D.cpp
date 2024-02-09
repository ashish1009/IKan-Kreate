//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#include "Renderer2D.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/2D/BatchData.h"
#include "Renderer/2D/QuadData.h"
#include "Renderer/2D/CircleData.h"
#include "Renderer/2D/LineData.h"

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_LOG_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_LOG_WARN(LogModule::Renderer2D, __VA_ARGS__)

  static constexpr glm::vec2 TextureCoords[] =
  {
    { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
  };

  struct Renderer2DData
  {
    QuadBatchData quadData;
    CircleBatchData circleData;
    LineBatchData lineData;
    FullScreenQuad fullScreenQuad;
    
    glm::mat4 cameraViewProjectionMatrix;
    glm::mat4 cameraViewMatrix;
    
    void Destroy()
    {
      quadData.Destroy();
      circleData.Destroy();
      lineData.Destroy();
      fullScreenQuad.Destroy();
    }
  };
  static Renderer2DData s_data;

  void Renderer2D::Initialize(const Renderer2DBatchLimit& data)
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
    
    AddQuadData(data.maxQuads);
    AddCircleData(data.maxCircles);
    AddLineData(data.maxLines);
    s_data.fullScreenQuad.Initialize();
  }
  
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_INFO("Shutting Down the Batch Renderer 2D ");
    
    s_data.Destroy();
  }

  void Renderer2D::AddQuadData(uint32_t maxQuads)
  {
    IK_PROFILE();
    RETURN_IF(maxQuads == 0);
    s_data.quadData.Initialize(maxQuads);
    RendererStatistics::Get()._2d.maxQuads = s_data.quadData.maxElement;
  }

  void Renderer2D::AddCircleData(uint32_t maxCircles)
  {
    IK_PROFILE();
    RETURN_IF(maxCircles == 0);
    s_data.circleData.Initialize(maxCircles);
    RendererStatistics::Get()._2d.maxCircles = s_data.circleData.maxElement;
  }

  void Renderer2D::AddLineData(uint32_t maxLines)
  {
    IK_PROFILE();
    RETURN_IF(maxLines == 0);
    s_data.lineData.Initialize(maxLines);
    RendererStatistics::Get()._2d.maxLines = s_data.lineData.maxElement;
  }
  
  void Renderer2D::BeginBatch(const glm::mat4 &camViewProjMat, const glm::mat4 &cameraViewMat)
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.cameraViewProjectionMatrix = camViewProjMat;
    s_data.cameraViewMatrix = cameraViewMat;
    
    s_data.quadData.StartBatch(camViewProjMat);
    s_data.circleData.StartBatch(camViewProjMat);
    s_data.lineData.StartBatch(camViewProjMat);
  }
  
  void Renderer2D::EndBatch()
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.quadData.Flush();
    s_data.circleData.Flush();
    s_data.lineData.Flush();
  }
  
  void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& tintColor, const Ref<Texture>& texture, float tilingFactor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawQuad (With Transform)");
    DrawTextureQuad(transform, texture ? texture : nullptr, TextureCoords, tilingFactor, tintColor, objectID );
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& rotation, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawQuad (With PSR)");
    auto transform = Utils::Math::GetTransformMatrix(position, rotation, glm::vec3(scale, 0.0f));
    DrawTextureQuad(transform, texture, TextureCoords, tilingFactor, color, objectID);
  }
  
  void Renderer2D::DrawFullscreenQuad(const Ref<Texture>& texture, uint32_t slot, bool overrideShader)
  {
    IK_PERFORMANCE("Renderer2D::DrawFullscreenQuad");
    // Bind the default Shader
    if (!overrideShader)
    {
      s_data.fullScreenQuad.shader->Bind();
    }
    
    if (texture)
    {
      texture->Bind(slot);
    }
    else
    {
      s_data.fullScreenQuad.whiteTexture->Bind();
    }
    Renderer::DrawQuad(s_data.fullScreenQuad.pipeline);
    
    // Unbind the default Shader
    if (!overrideShader)
    {
      s_data.fullScreenQuad.shader->Unbind();
    }
  }
  
  void Renderer2D::DrawFixedViewQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,
                                     float tilingFactor, int32_t entityID)
  {
    IK_PERFORMANCE("Renderer2D::DrawTextureQuad (With Transform)");
    glm::vec3 position, rotation, scale;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    DrawFixedViewQuad(position, scale, texture, tintColor, tilingFactor, entityID);
  }
  
  void Renderer2D::DrawFixedViewQuad(const glm::vec3& position, const glm::vec2& scale, const Ref<Texture>& texture,
                                     const glm::vec4& tintColor, float tilingFactor, int32_t entityID)
  {
    IK_PERFORMANCE("Renderer2D::DrawTextureQuad (With PSR)");
    // If number of indices increase in batch then start new batch
    if (s_data.quadData.indexCount >= s_data.quadData.maxIndices)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_data.quadData.indexCount);
      EndBatch();
      s_data.quadData.ResetBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_data.quadData.textureSlotIndex; i++)
      {
        if (s_data.quadData.textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_data.quadData.textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_data.quadData.textureSlotIndex);
          EndBatch();
          s_data.quadData.ResetBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_data.quadData.textureSlotIndex;
        s_data.quadData.textureSlots[s_data.quadData.textureSlotIndex] = texture;
        s_data.quadData.textureSlotIndex++;
      }
    }
    
    // get the fixed view from camera view matrix
    glm::vec3 camRightWS =
    {
      s_data.cameraViewMatrix[0][0],
      s_data.cameraViewMatrix[1][0],
      s_data.cameraViewMatrix[2][0]
    };
    
    glm::vec3 camUpWS =
    {
      s_data.cameraViewMatrix[0][1],
      s_data.cameraViewMatrix[1][1],
      s_data.cameraViewMatrix[2][1]
    };
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++)
    {
      s_data.quadData.vertexBufferPtr->position     = position + camRightWS *
      s_data.quadData.vertexBasePosition[i].x * scale.x + camUpWS *
      s_data.quadData.vertexBasePosition[i].y * -scale.y;
      
      s_data.quadData.vertexBufferPtr->color            = tintColor;
      s_data.quadData.vertexBufferPtr->textureCoords    = TextureCoords[i];
      s_data.quadData.vertexBufferPtr->textureIndex     = textureIndex;
      s_data.quadData.vertexBufferPtr->tilingFactor     = tilingFactor;
      s_data.quadData.vertexBufferPtr->pixelID          = entityID;
      s_data.quadData.vertexBufferPtr++;
    }
    
    s_data.quadData.indexCount += Shape2DCommonData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }
  
  void Renderer2D::DrawTextureQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec2* textureCoords,
                                   float tilingFactor, const glm::vec4& tintColor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawTextureQuad");
    // If number of indices increase in batch then start new batch
    if (s_data.quadData.indexCount >= s_data.quadData.maxIndices)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_data.quadData.indexCount);
      EndBatch();
      s_data.quadData.ResetBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_data.quadData.textureSlotIndex; i++)
      {
        if (s_data.quadData.textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_data.quadData.textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_data.quadData.textureSlotIndex);
          EndBatch();
          s_data.quadData.ResetBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_data.quadData.textureSlotIndex;
        s_data.quadData.textureSlots[s_data.quadData.textureSlotIndex] = texture;
        s_data.quadData.textureSlotIndex++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++)
    {
      s_data.quadData.vertexBufferPtr->position        = transform * s_data.quadData.vertexBasePosition[i];
      s_data.quadData.vertexBufferPtr->color           = tintColor;
      s_data.quadData.vertexBufferPtr->textureCoords   = textureCoords[i];
      s_data.quadData.vertexBufferPtr->textureIndex    = textureIndex;
      s_data.quadData.vertexBufferPtr->tilingFactor    = tilingFactor;
      s_data.quadData.vertexBufferPtr->pixelID         = objectID;
      s_data.quadData.vertexBufferPtr++;
    }
    
    s_data.quadData.indexCount += Shape2DCommonData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }

  void Renderer2D::DrawTextureCircle(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                                     const glm::vec4& tintColor, float thickness, float fade, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawTextureCircle");
    // If number of indices increase in batch then start new batch
    if (s_data.circleData.indexCount >= s_data.circleData.maxIndices)
    {
      BATCH_INFO("Starts the new batch as number of indices ({0}) increases in the previous batch", s_data.circleData.indexCount);
      EndBatch();
      s_data.circleData.ResetBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_data.circleData.textureSlotIndex; i++)
      {
        if (s_data.circleData.textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_data.circleData.textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_INFO("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_data.circleData.textureSlotIndex);
          EndBatch();
          s_data.circleData.ResetBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_data.circleData.textureSlotIndex;
        s_data.circleData.textureSlots[s_data.circleData.textureSlotIndex] = texture;
        s_data.circleData.textureSlotIndex++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++)
    {
      s_data.circleData.vertexBufferPtr->position        = transform * s_data.circleData.vertexBasePosition[i];
      s_data.circleData.vertexBufferPtr->color           = tintColor;
      s_data.circleData.vertexBufferPtr->textureCoords   = 2.0f * s_data.circleData.vertexBasePosition[i];
      s_data.circleData.vertexBufferPtr->textureIndex    = textureIndex;
      s_data.circleData.vertexBufferPtr->tilingFactor    = tilingFactor;
      s_data.circleData.vertexBufferPtr->localPosition   = 2.0f * s_data.circleData.vertexBasePosition[i];
      s_data.circleData.vertexBufferPtr->thickness       = thickness;
      s_data.circleData.vertexBufferPtr->fade            = fade;
      s_data.circleData.vertexBufferPtr->pixelID         = objectID;
      s_data.circleData.vertexBufferPtr++;
    }
    
    s_data.circleData.indexCount += Shape2DCommonData::IndicesForSingleElement;
    
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.circles++;
  }
} // namespace IKan
