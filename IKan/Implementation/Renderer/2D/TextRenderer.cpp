//
//  TextRenderer.cpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#include "TextRenderer.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/RendererStats.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/RendererBuffer.hpp"
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
      delete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  static Scope<TextData> s_textData;
  
  void TextRenderer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::TextRenderer, "Initialising the Text Renderer ");
    
    s_textData = CreateScope<TextData>();
  }
  
  void TextRenderer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::TextRenderer, "Shutting down the Text Renderer ");
    
    s_textData.reset();
  }
}
