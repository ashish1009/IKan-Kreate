//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 10/07/24.
//

#include "Renderer2D.hpp"

#include "Renderer/QuadData.hpp"
#include "Renderer/CircleData.hpp"
#include "Renderer/LineData.hpp"

namespace IKan
{
  static constexpr glm::vec2 TextureCoords[] =
  {
    { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
  };

  /// This stucture stores the Renderer 2D Data
  struct Renderer2DData
  {
    glm::mat4 cameraViewProjectionMatrix;
    glm::mat4 cameraViewMatrix;

    QuadBatchData quadData;
    CircleBatchData circleData;
    LineBatchData lineData;

    void Destroy()
    {
      IK_PROFILE();
      
      quadData.Destroy();
      circleData.Destroy();
      lineData.Destroy();
    }
  };
  static Renderer2DData s_data;
  
  void Renderer2D::Initialize()
  {
    IK_PROFILE();
    BATCH_INFO("Initialising the Batch Renderer 2D ");
    
    AddQuads(1000);
    AddCircles(1000);
    AddLines(10000);
  }
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_WARN("Shutting Down the Batch Renderer 2D ");
    
    s_data.Destroy();
  }
  
  void Renderer2D::AddQuads(uint32_t quads)
  {
    IK_PROFILE();
    if (quads == 0)
    {
      return;
    }
    
    s_data.quadData.Initialize(quads);
  }
  
  void Renderer2D::AddCircles(uint32_t circles)
  {
    IK_PROFILE();
    if (circles == 0)
    {
      return;
    }
    
    s_data.circleData.Initialize(circles);
  }
  
  void Renderer2D::AddLines(uint32_t lines)
  {
    IK_PROFILE();
    if (lines == 0)
    {
      return;
    }
    
    s_data.lineData.Initialize(lines);
  }
  
  void Renderer2D::BeginBatch(const glm::mat4 &cameraViewProjectionMatrix, const glm::mat4 &cameraViewMatrix)
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.cameraViewProjectionMatrix = cameraViewProjectionMatrix;
    s_data.cameraViewMatrix = cameraViewMatrix;
    
    s_data.quadData.StartBatch(s_data.cameraViewProjectionMatrix);
    s_data.circleData.StartBatch(s_data.cameraViewProjectionMatrix);
    s_data.lineData.StartBatch(s_data.cameraViewProjectionMatrix);
  }
  
  void Renderer2D::EndBatch()
  {
    IK_PERFORMANCE("Renderer2D::BeginBatch");
    s_data.quadData.Flush();
    s_data.circleData.Flush();
    s_data.lineData.Flush();
  }
  
  void Renderer2D::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawQuad (With Transform)");
    SubmitQuadImpl(transform, color, texture ? texture : nullptr, tilingFactor, TextureCoords, objectID );
  }
  void Renderer2D::SubmitQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& rotation, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawQuad (With PSR)");
    const glm::mat4& transform = Utils::Math::GetTransformMatrix(position, rotation, glm::vec3(scale, 0.0f));
    SubmitQuadImpl(transform, color, texture, tilingFactor, TextureCoords, objectID);
  }

  void Renderer2D::SubmitQuadImpl(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture,
                                  float tilingFactor, const glm::vec2* textureCoords, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::DrawTextureQuad");
    
    // If number of indices increase in batch then start new batch
    if (s_data.quadData.indexCountInBatch >= s_data.quadData.maxIndicesPerBatch)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_data.quadData.indexCountInBatch);
      s_data.quadData.Flush();
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
        if (s_data.quadData.textureSlotIndex >= Texture2DSpecification::MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_data.quadData.textureSlotIndex);
          s_data.quadData.Flush();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_data.quadData.textureSlotIndex;
        s_data.quadData.textureSlots[s_data.quadData.textureSlotIndex] = texture;
        s_data.quadData.textureSlotIndex++;
      }
    }
    
    for (size_t i = 0; i < Shape2DData::VertexForSingleElement; i++)
    {
      s_data.quadData.vertexBufferPtr->position        = transform * s_data.quadData.vertexBasePosition[i];
      s_data.quadData.vertexBufferPtr->color           = color;
      s_data.quadData.vertexBufferPtr->textureCoords   = textureCoords[i];
      s_data.quadData.vertexBufferPtr->textureIndex    = textureIndex;
      s_data.quadData.vertexBufferPtr->tilingFactor    = tilingFactor;
      s_data.quadData.vertexBufferPtr->pixelID         = objectID;
      s_data.quadData.vertexBufferPtr++;
    }
    
    s_data.quadData.indexCountInBatch += Shape2DData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }
} // namespace IKan
