//
//  LineData.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/BatchData.hpp"

namespace IKan
{
  /// Batch Data to Rendering Lines
  struct LineBatchData : CommonBatchData
  {
    // Constants
    static constexpr uint32_t VertexForSingleLine = 2;
    
    /// Count of Indices to be renderer in Single Batch
    uint32_t vertexCount = 0;
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    void Initialize(uint32_t lines)
    {
      IK_PROFILE();
      if (lines == 0)
      {
        return;
      }
      
      // If data already allocated then reset first
      if (maxElementPerBatch != 0)
      {
        lines += maxElementPerBatch;
        this->Destroy();
      }
      
      // Initialize the data for Common shape
      maxElementPerBatch = lines;
      maxVerticesPerBatch = maxElementPerBatch * LineBatchData::VertexForSingleLine;
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = new LineBatchData::Vertex[maxVerticesPerBatch];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(maxVerticesPerBatch * sizeof(LineBatchData::Vertex));
      
      // Create Pipeline
      PipelineSpecification pipelineSpec;
      pipelineSpec.debugName = "Line Renderer";
      pipelineSpec.vertexLayout =
      {
        { "a_Position", ShaderDataType::Float3 },
        { "a_Color",    ShaderDataType::Float4 },
      };
      
      // Create the Pipeline instnace
      pipeline = PipelineFactory::Create(pipelineSpec);
      shader = ShaderLibrary::Get(CoreAsset("Shaders/BatchLineShader.glsl"));
      pipelineSpec.shader = shader;
      
      RendererStatistics::Get()._2d.maxLines = maxElementPerBatch;
      
      BATCH_INFO("Initializing Batch Renderer for Line Data ");
      BATCH_INFO("  Max Lines per Batch             | {0}", maxElementPerBatch);
      BATCH_INFO("  Vertex Buffer used              | {0} B", maxVerticesPerBatch * sizeof(CommonBatchData::Vertex));
      BATCH_INFO("  Shader Used                     | {0}", shader->GetName());
    }
    void Destroy()
    {
      IK_PROFILE();
      if (maxElementPerBatch > 0)
      {
        BATCH_WARN("Destrying Batch Renderer for Line Data ");
        BATCH_WARN("  Max Lines per Batch             | {0}", maxElementPerBatch);
        BATCH_WARN("  Vertex Buffer used              | {0} B", maxVerticesPerBatch * sizeof(CommonBatchData::Vertex));
        BATCH_WARN("  Shader Used                     | {0}", shader->GetName());
        
        RendererStatistics::Get()._2d.maxLines -= maxElementPerBatch;
      }
      
      CommonBatchData::Destroy();
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
} // namespace IKan
