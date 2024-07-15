//
//  TextData.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan
{
  /// This structure stores the Text renderer data
  struct TextData
  {
    /// This structure stores the vertex information for rendering Text
    struct Vertex
    {
      glm::vec3 position;
      glm::vec4 color;
      glm::vec2 textureCoord;
      float textureIndex;
      int32_t objectID;
    };

    // Fixed Constants
    static constexpr uint32_t VertexForSingleChar = 6;

    /// Renderer Data storage
    Ref<Pipeline> pipeline;
    Ref<VertexBuffer> vertexBuffer;
    Ref<Shader> shader;

    /// Stores the Char Textures in array
    std::array<Ref<CharTexture>, Texture2DSpecification::MaxTextureSlotsInShader> charTextures;

    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    /// Store the Base Texture coordinga
    glm::vec2 baseTextureCoords[VertexForSingleChar];
    
    uint32_t numSlotsUsed = 0;

    void Initialize()
    {
      IK_PROFILE();
      
      // Allocating the memory for vertex Buffer Pointer
      vertexBufferBasePtr = new TextData::Vertex[TextData::VertexForSingleChar * Texture2DSpecification::MaxTextureSlotsInShader];
      
      // Create vertes Buffer
      vertexBuffer = VertexBufferFactory::Create(sizeof(TextData::Vertex) * TextData::VertexForSingleChar * Texture2DSpecification::MaxTextureSlotsInShader);
      
      // Create Pipeline specification
      PipelineSpecification pipelineSpec;
      pipelineSpec.debugName = "Text Renderer";
      pipelineSpec.vertexLayout =
      {
        { "a_Position",  ShaderDataType::Float3 },
        { "a_Color",     ShaderDataType::Float4 },
        { "a_TexCoords", ShaderDataType::Float2 },
        { "a_TexIndex",  ShaderDataType::Float },
        { "a_ObjectID",  ShaderDataType::Int },
      };
      shader = ShaderFactory::Create(CoreAsset("Shaders/TextShader.glsl"));
      pipelineSpec.shader = shader;
      
      // Create the Pipeline instnace
      pipeline = PipelineFactory::Create(pipelineSpec);
      
      // Base Texture coordinate for Char rendering
      baseTextureCoords[0] = { 0.0f, 0.0f };
      baseTextureCoords[1] = { 0.0f, 1.0f };
      baseTextureCoords[2] = { 1.0f, 1.0f };
      baseTextureCoords[3] = { 0.0f, 0.0f };
      baseTextureCoords[4] = { 1.0f, 1.0f };
      baseTextureCoords[5] = { 1.0f, 0.0f };
      
      IK_LOG_INFO(LogModule::TextRenderer, "Initialized the Text Renderer ");
      IK_LOG_INFO(LogModule::TextRenderer, "  Vertex Buffer Used  | {0} B",
                  TextData::VertexForSingleChar * sizeof(TextData::Vertex) * Texture2DSpecification::MaxTextureSlotsInShader);
      IK_LOG_INFO(LogModule::TextRenderer, "  Shader used         | {0}", shader->GetName());
    }
    
    void Destroy()
    {
      IK_PROFILE();
      
      IK_LOG_INFO(LogModule::TextRenderer, "Shutting down the Text Renderer ");
      IK_LOG_INFO(LogModule::TextRenderer, "  Vertex Buffer Used  | {0} B",
                  TextData::VertexForSingleChar * sizeof(TextData::Vertex) * Texture2DSpecification::MaxTextureSlotsInShader);
      IK_LOG_INFO(LogModule::TextRenderer, "  Shader used         | {0}", shader->GetName());
      
      vertexBuffer.reset();
      pipeline.reset();
      shader.reset();
      
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
    
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Text::StartBatch");
      
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camViewProjMat);
      shader->Unbind();
      
      ResetBatch();
    }
    
    void ResetBatch()
    {
      IK_PERFORMANCE("Text::ResetBatch");
      vertexBufferPtr = vertexBufferBasePtr;
      numSlotsUsed = 0;
    }
    
    void Flush()
    {
      IK_PERFORMANCE("Text::EndBatch");
      uint32_t dataSize = (uint32_t)((uint8_t*)vertexBufferPtr - (uint8_t*)vertexBufferBasePtr);
      if (dataSize > 0)
      {
        vertexBuffer->SetData(vertexBufferBasePtr, dataSize);
        
        // Render the Scene
        shader->Bind();
        for (uint32_t j = 0; j < numSlotsUsed; j ++)
        {
          charTextures[j]->Bind(j);
        }
        Renderer::DrawArrays(pipeline, VertexForSingleChar * numSlotsUsed);
      }
      ResetBatch();
    }
  };
} // namespace IKan
