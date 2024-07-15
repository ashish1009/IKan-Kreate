//
//  CircleData.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/BatchData.hpp"

namespace IKan
{
  /// This structure stores the Circle data for 2D Batch rendering
  struct CircleBatchData : public Shape2DData
  {
    /// Single vertex of a Circle
    struct Vertex : Shape2DData::Vertex
    {
      glm::vec3 localPosition;
      float thickness;
      float fade;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    void Initialize(uint32_t circles)
    {
      IK_PROFILE();
      if (circles == 0) 
      {
        return;
      }
      
      // If data already allocated then reset first
      if (maxElementPerBatch != 0)
      {
        circles += maxElementPerBatch;
        this->Destroy();
      }
      
      // Initialize the Shape 2D Data
      Shape2DData::Initialize(circles);
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = new Vertex[maxVerticesPerBatch];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(maxVerticesPerBatch * sizeof(Vertex));
      
      // Create Pipeline specification
      PipelineSpecification pipelineSpec;
      pipelineSpec.debugName = "Circle Pipeline";
      pipelineSpec.vertexLayout =
      {
        { "a_Position",     ShaderDataType::Float3 },
        { "a_Color",        ShaderDataType::Float4 },
        { "a_TexCoords",    ShaderDataType::Float2 },
        { "a_TexIndex",     ShaderDataType::Float },
        { "a_TilingFactor", ShaderDataType::Float },
        { "a_ObjectID",     ShaderDataType::Int },
        { "a_LocalPosition",ShaderDataType::Float3 },
        { "a_Thickness",    ShaderDataType::Float },
        { "a_Fade",         ShaderDataType::Float },
      };
      
      // Create the Pipeline instnace
      pipeline = PipelineFactory::Create(pipelineSpec);
      shader = ShaderLibrary::Get(CoreAsset("Shaders/BatchCircleShader.glsl"));
      pipelineSpec.shader = shader;
      
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
      RendererStatistics::Get()._2d.maxCircles = maxElementPerBatch;
      
      // Debug Logs
      BATCH_INFO("Initialising Batch Renderer for Circle Data ");
      BATCH_INFO("  Max Circles per Batch          | {0}", maxElementPerBatch);
      BATCH_INFO("  Vertex Buffer used             | {0} B", maxVerticesPerBatch * sizeof(Vertex));
      BATCH_INFO("  Index Buffer used              | {0} B", maxIndicesPerBatch * sizeof(uint32_t));
      BATCH_INFO("  Shader Used                    | {0}", shader->GetName());
    }
    
    void Destroy()
    {
      IK_PROFILE();
      if (maxElementPerBatch > 0)
      {
        BATCH_WARN("Destrying Batch Renderer for Circle Data ");
        BATCH_WARN("  Max Circles per Batch           | {0}", maxElementPerBatch);
        BATCH_WARN("  Vertex Buffer used              | {0} B", maxVerticesPerBatch * sizeof(Shape2DData::Vertex));
        BATCH_WARN("  Index Buffer used               | {0} B", maxIndicesPerBatch * sizeof(uint32_t));
        BATCH_WARN("  Shader Used                     | {0}", shader->GetName());
        
        // Decrease the stat
        RendererStatistics::Get()._2d.maxCircles -= maxElementPerBatch;
      }
      
      Shape2DData::Destroy();
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Circle::StartBatch");
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camViewProjMat);
      shader->Unbind();
      
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Circle::ResetBatch");
      
      indexCountInBatch = 0;
      textureSlotIndex = 1;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Circle::Flush");
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
