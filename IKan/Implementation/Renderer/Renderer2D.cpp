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
    IK_PERFORMANCE("Renderer2D::SubmitQuad (With Transform)");
    SubmitQuadImpl(transform, color, texture, tilingFactor, TextureCoords, objectID );
  }
  void Renderer2D::SubmitQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec3& rotation, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitQuad (With PSR)");
    const glm::mat4& transform = Utils::Math::GetTransformMatrix(position, rotation, glm::vec3(scale, 0.0f));
    SubmitQuadImpl(transform, color, texture, tilingFactor, TextureCoords, objectID);
  }

  void Renderer2D::SubmitQuadImpl(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture,
                                  float tilingFactor, const glm::vec2* textureCoords, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitQuadImpl");
    
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
  
  void Renderer2D::SubmitCircle(const glm::vec3& position, float radius, const glm::vec3& rotation, const glm::vec4& color,
                                const Ref<Texture>& texture, float tilingFactor, float thickness, float fade, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitCircle (With PSR)");
    const glm::mat4& transform = Utils::Math::GetTransformMatrix(position, rotation, {radius, radius, radius});
    SubmitCircleImpl(transform, texture, tilingFactor, color, thickness, fade, objectID);
  }
  
  void Renderer2D::SubmitCircle(const glm::vec3& position, const glm::vec2& radius, const glm::vec3& rotation, const glm::vec4& color,
                                const Ref<Texture>& texture, float tilingFactor, float thickness, float fade, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitCircle (With PSR)");
    const glm::mat4& transform = Utils::Math::GetTransformMatrix(position, rotation, glm::vec3(radius, 0.0f));
    SubmitCircleImpl(transform, texture, tilingFactor, color, thickness, fade, objectID);
  }
  
  void Renderer2D::SubmitCircle(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture>& texture, float tilingFactor,
                                float thickness, float fade, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitCircle (With Transform)");
    SubmitCircleImpl(transform, texture, tilingFactor, color, thickness, fade, objectID);
  }
  
  void Renderer2D::SubmitCircleImpl(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                                    const glm::vec4& tintColor, float thickness, float fade, int32_t objectID)
  {
    IK_PERFORMANCE("Renderer2D::SubmitCircleImpl");
    
    // If number of indices increase in batch then start new batch
    if (s_data.circleData.indexCountInBatch >= s_data.circleData.maxIndicesPerBatch)
    {
      BATCH_INFO("Starts the new batch as number of indices ({0}) increases in the previous batch", s_data.circleData.indexCountInBatch);
      s_data.circleData.Flush();
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
        if (s_data.circleData.textureSlotIndex >= Texture2DSpecification::MaxTextureSlotsInShader)
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
    
    for (size_t i = 0; i < Shape2DData::VertexForSingleElement; i++)
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
    
    s_data.circleData.indexCountInBatch += Shape2DData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DData::VertexForSingleElement;
    RendererStatistics::Get()._2d.circles++;
  }
  
  void Renderer2D::SubmitLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer2D::DrawLine");
    // If number of indices increase in batch then start new batch
    if (s_data.lineData.vertexCount >= s_data.lineData.maxVerticesPerBatch)
    {
      BATCH_INFO("Starts the new batch as number of vertices ({0}) increases in the previous batch", s_data.lineData.vertexCount);
      s_data.lineData.Flush();
    }
    
    s_data.lineData.vertexBufferPtr->position = p0;
    s_data.lineData.vertexBufferPtr->color = color;
    s_data.lineData.vertexBufferPtr++;
    
    s_data.lineData.vertexBufferPtr->position = p1;
    s_data.lineData.vertexBufferPtr->color = color;
    s_data.lineData.vertexBufferPtr++;
    
    s_data.lineData.vertexCount += LineBatchData::VertexForSingleLine;
    RendererStatistics::Get().vertexCount += LineBatchData::VertexForSingleLine;
    RendererStatistics::Get()._2d.lines++;
  }
  
  void Renderer2D::SubmitRect(const glm::vec3& p0, const glm::vec3& p2, const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer2D::DrawRect (With 2 Points)");
    glm::vec3 p1 = glm::vec3(p2.x, p0.y, p0.z);
    glm::vec3 p3 = glm::vec3(p0.x, p2.y, p2.z);
    
    SubmitRect(p0, p1, p2, p3, color);
  }
  
  void Renderer2D::SubmitRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer2D::DrawRect (With Position)");
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    
    SubmitRect(p0, p1, p2, p3, color);
  }
  
  
  void Renderer2D::SubmitRect(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
                            const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer2D::DrawRect (With 4 Points)");
    SubmitLine(p0, p1, color);
    SubmitLine(p1, p2, color);
    SubmitLine(p2, p3, color);
    SubmitLine(p3, p0, color);
  }
  
  void Renderer2D::SubmitRect(const glm::mat4& transform, const glm::vec4& color)
  {
    IK_PERFORMANCE("Renderer2D::DrawRect (With Transform)");
    glm::vec3 lineVertices[4];
    for (size_t i = 0; i < 4; i++)
    {
      lineVertices[i] = transform * s_data.quadData.vertexBasePosition[i];
    }
    
    SubmitLine(lineVertices[0], lineVertices[1], color);
    SubmitLine(lineVertices[1], lineVertices[2], color);
    SubmitLine(lineVertices[2], lineVertices[3], color);
    SubmitLine(lineVertices[3], lineVertices[0], color);
  }
} // namespace IKan
