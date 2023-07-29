//
//  TextRenderer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "TextRenderer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// This structure stores the Text renderer data
  struct TextData
  {
    /// Vertex Information for rendering Text
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
    std::array<Ref<CharTexture>, MaxTextureSlotsInShader> charTextures;
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    /// Store the Base Texture coordinga
    glm::vec2 baseTextureCoords[VertexForSingleChar];
    
    uint32_t numSlotsUsed = 0;
    
    virtual ~TextData()
    {
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  static Scope<TextData> s_textData;
  
  void TextRenderer::Initialise()
  {
    IK_LOG_TRACE(LogModule::TextRenderer, "Initialising the Text Renderer ");
    IK_LOG_TRACE(LogModule::TextRenderer, "-------------------------------");
    
    s_textData = CreateScope<TextData>();
    
    // Allocating the memory for vertex Buffer Pointer
    s_textData->vertexBufferBasePtr = iknew TextData::Vertex[TextData::VertexForSingleChar * MaxTextureSlotsInShader];
    
    // Create vertes Buffer
    s_textData->vertexBuffer = VertexBuffer::Create(sizeof(TextData::Vertex) * TextData::VertexForSingleChar * MaxTextureSlotsInShader);
    
    // Create Pipeline specification
    Pipeline::Specification pipelineSpec;
    pipelineSpec.debugName = "Text Renderer";
    pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/TextShader.glsl"));
    pipelineSpec.layout =
    {
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Color",     ShaderDataType::Float4 },
      { "a_TexCoords", ShaderDataType::Float2 },
      { "a_TexIndex",  ShaderDataType::Float },
      { "a_ObjectID",  ShaderDataType::Int },
    };
    
    // Create the Pipeline instnace
    s_textData->pipeline = Pipeline::Create(pipelineSpec);
    
    // Setup the Quad Shader. Copy the Same shader refernce which is stored in the Pipeline
    s_textData->shader = s_textData->pipeline->GetSpecification().shader;
    
    // Base Texture coordinate for Char rendering
    s_textData->baseTextureCoords[0] = { 0.0f, 0.0f };
    s_textData->baseTextureCoords[1] = { 0.0f, 1.0f };
    s_textData->baseTextureCoords[2] = { 1.0f, 1.0f };
    s_textData->baseTextureCoords[3] = { 0.0f, 0.0f };
    s_textData->baseTextureCoords[4] = { 1.0f, 1.0f };
    s_textData->baseTextureCoords[5] = { 1.0f, 0.0f };
    
    IK_LOG_TRACE(LogModule::TextRenderer, "Initialised the Text Renderer ");
    IK_LOG_TRACE(LogModule::TextRenderer, "------------------------------");
    IK_LOG_TRACE(LogModule::TextRenderer, "  Vertex Buffer Used  {0} B",
                 TextData::VertexForSingleChar * sizeof(TextData::Vertex) * MaxTextureSlotsInShader);
    IK_LOG_TRACE(LogModule::TextRenderer, "  Shader used         {0}", s_textData->shader->GetName());
  }
  
  void TextRenderer::Shutdown()
  {
    IK_LOG_WARN(LogModule::TextRenderer, "Shutting down the Text Renderer ");
    IK_LOG_WARN(LogModule::TextRenderer, "--------------------------------");
    IK_LOG_WARN(LogModule::TextRenderer, "  Vertex Buffer Used  {0} B",
                TextData::VertexForSingleChar * sizeof(TextData::Vertex) * MaxTextureSlotsInShader);
    IK_LOG_WARN(LogModule::TextRenderer, "  Shader used         {0}", s_textData->shader->GetName());
    
    s_textData.reset();
  }
  
  void TextRenderer::BeginBatch(const glm::mat4 &camViewProjMat)
  {
    // Update camera to shader
    s_textData->shader->Bind();
    s_textData->shader->SetUniformMat4("v_Projection", camViewProjMat);
    
    NextBatch();
  }
  
  void TextRenderer::EndBatch()
  {
    uint32_t dataSize = (uint32_t)((uint8_t*)s_textData->vertexBufferPtr - (uint8_t*)s_textData->vertexBufferBasePtr);
    s_textData->vertexBuffer->SetData( s_textData->vertexBufferBasePtr, dataSize);
    
    // Render the Scene
    s_textData->shader->Bind();
    for (int j = 0; j < s_textData->numSlotsUsed; j ++)
    {
      s_textData->charTextures[j]->Bind(j);
    }
    Renderer::DrawArrays(s_textData->pipeline, 6 * s_textData->numSlotsUsed);
  }
  
  void TextRenderer::NextBatch()
  {
    s_textData->vertexBufferPtr = s_textData->vertexBufferBasePtr;
    s_textData->numSlotsUsed = 0;
  }
  
  void TextRenderer::Flush() {
    EndBatch();
    NextBatch();
  }

}
