//
//  CircleData.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
  /// Batch Data to Rendering Circles
  struct CircleBatchData : Shape2DCommonData
  {
    /// Single vertex of a Circle
    struct Vertex : CommonVertex
    {
      glm::vec3 localPosition;
      float thickness;
      float fade;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Circle::StartBatch");
      LoadCamToShader(shader, camViewProjMat);
      indexCount = 0;
      textureSlotIndex = 1;
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Circle::ResetBatch");
      indexCount = 0;
      textureSlotIndex = 1;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Circle::Flush");
      if (indexCount)
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
        Renderer::DrawIndexed(pipeline, indexCount);
      }
    }
    
    void Initialize(uint32_t maxCircles)
    {
      IK_PROFILE();
      // If data already allocated then reset first
      if (maxElement != 0)
      {
        maxCircles += maxElement;
        this->Destroy();
      }
      
      // Initialize the Shape 2D Data
      InitializeShapeData(maxCircles);
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = new Vertex[maxVertices];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(maxVertices * sizeof(Vertex));
      
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
      
      // Create Index Buffer
      uint32_t* indices = new uint32_t[maxIndices];
      uint32_t offset = 0;
      for (size_t i = 0; i < maxIndices; i += Shape2DCommonData::IndicesForSingleElement)
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
      indexBuffer = IndexBufferFactory::CreateWithCount(indices, maxIndices);
      delete[] indices;
      
      // Setup the Quad Shader. Copy the Same shader refernce which is stored in the Pipeline
      shader = ShaderLibrary::GetShader(CoreAsset("Shaders/BatchCircleShader.glsl"));
      
      // Debug Logs
      BATCH_INFO("Initialized Batch Renderer for Circle Data ");
      BATCH_INFO("  Max Quads per Batch              {0}", maxElement);
      BATCH_INFO("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
      BATCH_INFO("  Vertex Buffer used               {0} B", maxVertices * sizeof(Shape2DCommonData::CommonVertex));
      BATCH_INFO("  Index Buffer used                {0} B", maxIndices * sizeof(uint32_t));
      BATCH_INFO("  Shader Used                      {0}", shader->GetName());
    }
    
    void Destroy()
    {
      IK_PROFILE();
      if (maxElement > 0)
      {
        BATCH_INFO("Destrying Batch Renderer for Circle Data ");
        BATCH_INFO("  Max Circle per Batch             {0}", maxElement);
        BATCH_INFO("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
        BATCH_INFO("  Vertex Buffer used               {0} B", maxVertices * sizeof(Shape2DCommonData::CommonVertex));
        BATCH_INFO("  Index Buffer used                {0} B", maxIndices * sizeof(uint32_t));
        BATCH_INFO("  Shader Used                      {0}", shader->GetName());
        
        RendererStatistics::Get()._2d.maxCircles -= maxElement;
      }
      
      Shape2DCommonData::Destroy();
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  
} // namespace IKan
