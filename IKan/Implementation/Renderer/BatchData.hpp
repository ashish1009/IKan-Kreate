//
//  BatchData.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_LOG_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_LOG_WARN(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_ERROR(...) IK_LOG_ERROR(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_CRITICAL(...) IK_LOG_CRITICAL(LogModule::Renderer2D, __VA_ARGS__)
  
  /// This structure stores the common data for all batch 2D renderer (Quads, Circles and Lines)
  struct CommonBatchData
  {
    // Member Variables ----------------------------------------------------------------------------------------------
    /// Batch limits
    uint32_t maxElementPerBatch {0};
    uint32_t maxVerticesPerBatch {0};

    // Member Functions ----------------------------------------------------------------------------------------------
    void Destroy()
    {
      IK_PROFILE();
      
      maxElementPerBatch = 0;
      maxVerticesPerBatch = 0;
    }
  };
  
  /// This structure stores the common data for 2D shape batch renderer (Quads and Circles)
  struct Shape2DData : CommonBatchData
  {
    // Constants -----------------------------------------------------------------------------------------------------
    static constexpr uint32_t VertexForSingleElement = 4;
    static constexpr uint32_t IndicesForSingleElement = 6;
    
    // Member Variables ----------------------------------------------------------------------------------------------
    /// Batch limits
    uint32_t maxIndicesPerBatch {0};
    uint32_t indexCountInBatch {0};

    /// Basic vertex of quad (Vertex of circle is taken as Quad only)
    glm::vec4 vertexBasePosition[4];
    
    /// Texture Slot index sent to Shader to render a specific Texture from slots
    /// - Note: Slot 0 is reserved for white texture (No Image only color)
    uint32_t textureSlotIndex {1};
    
    /// Stores all the 16 Texture in array so that there is no need to load texture each frame
    /// - Note: Load only if new texture is added or older replaced with new
    std::array<Ref<Texture>, Texture2DSpecification::MaxTextureSlotsInShader> textureSlots;

    // Member Functions ----------------------------------------------------------------------------------------------
    void Initialize(uint32_t maxElement)
    {
      IK_PROFILE();
      if (maxElement == 0)
      {
        return;
      }
      
      // Set the max limit of batch
      maxVerticesPerBatch = maxElement * VertexForSingleElement;
      maxIndicesPerBatch = maxElement * IndicesForSingleElement;
      CommonBatchData::maxElementPerBatch = maxElement;
      
      // Setting basic Vertex point of quad
      vertexBasePosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
      vertexBasePosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
      
      // Creating white texture for colorful (default white) quads witout any image file
      if (!textureSlots[0])
      {
        textureSlots[0] = TextureFactory::CreateWhiteTexture();
      }
    }
    
    void Destroy()
    {
      IK_PROFILE();
      maxIndicesPerBatch = 0;
      for (uint32_t i = 0; i < Texture2DSpecification::MaxTextureSlotsInShader; i++)
      {
        textureSlots[i].reset();
      }
      CommonBatchData::Destroy();
    }
  };
} // namespace IKan
