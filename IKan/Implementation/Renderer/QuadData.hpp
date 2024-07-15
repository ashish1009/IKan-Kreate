//
//  QuadData.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/BatchData.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan 
{
  /// This structure stores the quad data for single batch
  struct QuadBatchData : Shape2DData
  {
    // Member Variables ----------------------------------------------------------------------------------------------
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;

    // Member Functions ----------------------------------------------------------------------------------------------
    void Initialize(uint32_t quads)
    {
      IK_PROFILE();
      if (0 == quads)
      {
        return;
      }
      
      // If data already allocated then reset first
      if (maxElementPerBatch != 0)
      {
        quads += maxElementPerBatch;
        this->Destroy();
      }

      // Initialize common shape data
      Shape2DData::Initialize(quads);
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = new Shape2DData::Vertex[maxVerticesPerBatch];
      
      // Create vertes Buffer
      const uint32_t vertexBufferSize = maxVerticesPerBatch * sizeof(Shape2DData::Vertex);
      vertexBuffer = VertexBufferFactory::Create(vertexBufferSize);
      
      // Create Pipeline specification
      PipelineSpecification pipelineSpec;
      pipelineSpec.debugName = "Quad Pipeline";
      pipelineSpec.vertexLayout =
      {
        { "a_Position",     ShaderDataType::Float3 },
        { "a_Color",        ShaderDataType::Float4 },
        { "a_TexCoords",    ShaderDataType::Float2 },
        { "a_TexIndex",     ShaderDataType::Float },
        { "a_TilingFactor", ShaderDataType::Float },
        { "a_ObjectID",     ShaderDataType::Int },
      };
      shader = ShaderLibrary::Get(CoreAsset("Shaders/BatchQuadShader.glsl"));
      pipelineSpec.shader = shader;

      // Create the Pipeline instnace
      pipeline = PipelineFactory::Create(pipelineSpec);

      // Create Index Buffer
      uint32_t* indices = new uint32_t[maxIndicesPerBatch];
      uint32_t offset = 0;
      for (size_t i = 0; i < maxIndicesPerBatch; i += Shape2DData::IndicesForSingleElement)
      {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        
        offset += 4;
      }
      
      // Create Index Buffer in GPU for storing Indices. Once stores the date in GPU, we do not Need this Buffer
      indexBuffer = IndexBufferFactory::CreateWithCount(indices, maxIndicesPerBatch);
      delete[] indices;
      
      // Increase the stat
      RendererStatistics::Get()._2d.maxQuads = maxElementPerBatch;
      
      // Debug Logs
      BATCH_INFO("Initializing Batch Renderer for Quad Data ");
      BATCH_INFO("  Max Quads per Batch            | {0}", maxElementPerBatch);
      BATCH_INFO("  Vertex Buffer used             | {0} B", vertexBufferSize);
      BATCH_INFO("  Index Buffer used              | {0} B", maxIndicesPerBatch * sizeof(uint32_t));
      BATCH_INFO("  Shader Used                    | {0}", pipelineSpec.shader->GetName());
    }
    
    void Destroy()
    {
      IK_PROFILE();
      
      if (maxElementPerBatch > 0)
      {
        BATCH_WARN("Destrying Batch Renderer for Quad Data ");
        BATCH_WARN("  Max Quads per Batch           | {0}", maxElementPerBatch);
        BATCH_WARN("  Vertex Buffer used            | {0} B", maxVerticesPerBatch * sizeof(Shape2DData::Vertex));
        BATCH_WARN("  Index Buffer used             | {0} B", maxIndicesPerBatch * sizeof(uint32_t));
        BATCH_WARN("  Shader Used                   | {0}", shader->GetName());
        
        // Decrease the stat
        RendererStatistics::Get()._2d.maxQuads -= maxElementPerBatch;
      }

      Shape2DData::Destroy();
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Quad::StartBatch");
      
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camViewProjMat);
      shader->Unbind();
      
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Quad::ResetBatch");
      
      indexCountInBatch = 0;
      textureSlotIndex = 1;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Quad::Flush");
      if (indexCountInBatch)
      {
        // Set the vertex buffer data
        uint32_t dataSize = (uint32_t)((uint8_t*)vertexBufferPtr - (uint8_t*)vertexBufferBasePtr);
        vertexBuffer->SetData(vertexBufferBasePtr, dataSize);
        
        // Bind Textures
        for (uint32_t i = 0; i < textureSlotIndex; i++)
        {
          textureSlots[i]->Bind(i);
        }
        
        // Render the Data
        shader->Bind();
        Renderer::DrawIndexed(pipeline, indexCountInBatch);
      }
      ResetBatch();
    }
  };
} // namespace IKan
