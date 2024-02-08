//
//  LineData.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  /// Batch Data to Rendering Lines
  struct LineBatchData : CommonBatchData
  {
    // Constants
    static constexpr uint32_t VertexForSingleLine = 2;
    
    /// Single vertex of a Circle
    struct Vertex
    {
      glm::vec3 position;       // Position of a Quad
      glm::vec4 color;          // Color of a Quad
    };
    
    /// Count of Indices to be renderer in Single Batch
    uint32_t vertexCount = 0;
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Line::StartBatch");
      LoadCamToShader(shader, camViewProjMat);
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Line::ResetBatch");
      vertexCount = 0;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Line::Flush");
      if (vertexCount)
      {
        // Set the vertex buffer data
        uint32_t dataSize = (uint32_t)((uint8_t*)vertexBufferPtr - (uint8_t*)vertexBufferBasePtr);
        vertexBuffer->SetData(vertexBufferBasePtr, dataSize);
        
        // Render the Data
        shader->Bind();
        Renderer::DrawLines(pipeline, vertexCount);
      }
    }
    
    void Initialize(uint32_t maxLines)
    {
      IK_PROFILE();
      // If data already allocated then reset first
      if (maxElement != 0)
      {
        maxLines += maxElement;
        this->Destroy();
      }
      
      // Initialize the data for Common shape
      maxElement = maxLines;
      maxVertices = maxElement * LineBatchData::VertexForSingleLine;
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = iknew LineBatchData::Vertex[maxVertices];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(maxVertices * sizeof(LineBatchData::Vertex));
      
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
      
      // Setup the Quad Shader
      shader = ShaderLibrary::GetShader(CoreAsset("Shaders/BatchLineShader.glsl"));
      
      BATCH_INFO("Initialized Batch Renderer for Line Data ");
      BATCH_INFO("  Max Lines per Batch              {0}", maxElement);
      BATCH_INFO("  Vertex Buffer used               {0} B", maxVertices * sizeof(LineBatchData::Vertex));
      BATCH_INFO("  Shader Used                      {0}", shader->GetName());
    }
    void Destroy()
    {
      IK_PROFILE();
      if (maxElement > 0)
      {
        BATCH_INFO("Destrying Batch Renderer for Line Data ");
        BATCH_INFO("  Max Lines per Batch              {0}", maxElement);
        BATCH_INFO("  Vertex Buffer used               {0} B", maxVertices * sizeof(Shape2DCommonData::CommonVertex));
        BATCH_INFO("  Shader Used                      {0}", shader->GetName());
        
        RendererStatistics::Get()._2d.maxLines -= maxElement;
      }
      
      CommonBatchData::Destroy();
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
} // namespace IKan
