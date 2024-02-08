//
//  QuadData.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/2D/BatchData.h"

namespace IKan
{
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
  
  /// This structures stores the Full Screen quad
  struct FullScreenQuad
  {
    /// Vertices of Full Screen Quad
    static constexpr float vertices[] =
    {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    
    /// Pipelne to render full screen quad
    Ref<Pipeline> pipeline;
    Ref<VertexBuffer> vertexBuffer;
    Ref<Texture> whiteTexture;
    Ref<Shader> shader;
    
    /// This function initilaise the full screen quad data
    void Initialize()
    {
      IK_PROFILE();
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create((void*)&vertices, sizeof(vertices));
      
      // Create Pipeline specification
      PipelineSpecification pipelineSpec;
      pipelineSpec.debugName = "Full Screen Quad Pipeline";
      shader = ShaderLibrary::GetShader(CoreAsset("Shaders/FSQuadShader.glsl"));
      pipelineSpec.vertexLayout =
      {
        { "a_Position", ShaderDataType::Float3 },
        { "a_TexCoord", ShaderDataType::Float2 },
      };
      
      // Create the Pipeline instnace for full screen quad
      pipeline = PipelineFactory::Create(pipelineSpec);
      
      // Create white texture
      whiteTexture = TextureFactory::Create(0xffffffff);
      
      IK_LOG_INFO(LogModule::Renderer2D, "Initialized Fullscreen Quad Data ");
      IK_LOG_INFO(LogModule::Renderer2D, "  Vertex Buffer used               {0} B", sizeof(vertices));
      IK_LOG_INFO(LogModule::Renderer2D, "  Shader used                      {0}", shader->GetName());
    }
    
    void Destroy()
    {
      if (pipeline and vertexBuffer and whiteTexture)
      {
        IK_PROFILE();
        IK_LOG_INFO(LogModule::Renderer2D, "Destroying Fullscreen Quad Data ");
        IK_LOG_INFO(LogModule::Renderer2D, "  Vertex Buffer used               {0} B", sizeof(vertices));
        IK_LOG_INFO(LogModule::Renderer2D, "  Shader used                      {0}", shader->GetName());
        
        pipeline.reset();
        vertexBuffer.reset();
        whiteTexture.reset();
        shader.reset();
      }
    }
  };
  
  struct QuadBatchData : public Shape2DCommonData
  {
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    CommonVertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    CommonVertex* vertexBufferPtr = nullptr;
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Quad::StartBatch");
      LoadCamToShader(shader, camViewProjMat);
      indexCount = 0;
      textureSlotIndex = 1;
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Quad::ResetBatch");
      indexCount = 0;
      textureSlotIndex = 1;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Quad::Flush");
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
    
    void Initialize(uint32_t maxQuads)
    {
      IK_PROFILE();
      // If data already allocated then reset first
      if (maxElement != 0)
      {
        maxQuads += maxElement;
        this->Destroy();
      }
      
      // Initialize the Shape 2D Data
      InitializeShapeData(maxQuads);
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = iknew Shape2DCommonData::CommonVertex[maxVertices];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(maxVertices * sizeof(Shape2DCommonData::CommonVertex));
      
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
      
      // Create the Pipeline instnace
      pipeline = PipelineFactory::Create(pipelineSpec);
      
      // Create Index Buffer
      uint32_t* indices = iknew uint32_t[maxIndices];
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
      ikdelete[] indices;
      
      // Setup the Quad Shader. Copy the Same shader refernce which is stored in the Pipeline
      shader = ShaderLibrary::GetShader(CoreAsset("Shaders/BatchQuadShader.glsl"));
      
      // Debug Logs
      BATCH_INFO("Initialized Batch Renderer for Quad Data ");
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
        BATCH_INFO("Destrying Batch Renderer for Quad Data ");
        BATCH_INFO("  Max Quads per Batch              {0}", maxElement);
        BATCH_INFO("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
        BATCH_INFO("  Vertex Buffer used               {0} B", maxVertices * sizeof(Shape2DCommonData::CommonVertex));
        BATCH_INFO("  Index Buffer used                {0} B", maxIndices * sizeof(uint32_t));
        BATCH_INFO("  Shader Used                      {0}", shader->GetName());
        
        RendererStatistics::Get()._2d.maxQuads -= maxElement;
      }
      
      Shape2DCommonData::Destroy();
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
} // namespace IKan
