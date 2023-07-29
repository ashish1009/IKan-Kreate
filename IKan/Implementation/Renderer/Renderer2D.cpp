//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Renderer2D.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"
#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/RenderPass.hpp"

namespace IKan
{
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
    
    /// This function initilaise the full screen quad data
    FullScreenQuad()
    {
      IK_LOG_TRACE(LogModule::Renderer, "Initialising Full Screen Quad Data");
      
      // Create vertes Buffer
      vertexBuffer = VertexBuffer::Create((void*)&vertices, sizeof(vertices));
      
      // Create Pipeline specification
      Pipeline::Specification pipelineSpec;
      pipelineSpec.debugName = "Full Screen Quad Renderer";
      pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/FSQuadShader.glsl"));
      pipelineSpec.layout =
      {
        { "a_Position", ShaderDataType::Float3 },
        { "a_TexCoord", ShaderDataType::Float2 },
      };
      
      // Create the Pipeline instnace for full screen quad
      pipeline = Pipeline::Create(pipelineSpec);
      
      
      // White data
      static uint32_t whiteTextureData = 0xffffffff;
      
      // Texture specificaion
      Texture::Specification textureSpec;
      textureSpec.type = TextureType::Texture2D;
      textureSpec.internalFormat = TextureFormat::RGBA8;
      textureSpec.dataFormat = TextureFormat::RGBA;
      textureSpec.wrap = TextureWrap::Clamp;
      textureSpec.filter = TextureFilter::Linear;
      textureSpec.width = 1;
      textureSpec.height = 1;
      textureSpec.data = &whiteTextureData;
      textureSpec.size = sizeof(uint32_t);
      
      // Create white texture
      whiteTexture = Texture::Create(textureSpec);
      
      IK_LOG_TRACE(LogModule::Renderer, "Initialized Fullscreen Quad Data ");
      IK_LOG_TRACE(LogModule::Renderer, "-----------------------------------------");
      IK_LOG_TRACE(LogModule::Renderer, "  Vertex Buffer used               {0} B", sizeof(vertices));
      IK_LOG_TRACE(LogModule::Renderer, "  Shader used                      {0}", pipelineSpec.shader->GetName());
    }
  };
  static Scope<FullScreenQuad> s_fullscreenQuadData;
  
#define BATCH_INFO(...) IK_CORE_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_CORE_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_CORE_WARN(LogModule::Renderer2D, __VA_ARGS__)
  
  static constexpr glm::vec2 TextureCoords[] =
  {
    { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
  };
  
  /// This structure stores the common data for Batch 2D Renderer
  struct Renderer2DData
  {
    bool needResize = false;
    uint32_t viewportWidth, viewportHeight;
    Ref<RenderPass> renderPass;
  };
  static Scope<Renderer2DData> s_commonData;

  
  /// This structure holds the common batch renderer data for Quads, circle and lines
  struct CommonBatchData
  {
    // Lambda to load the camera data to shdaer
    const std::function<void(Ref<Shader>, const glm::mat4&)> loadCamToShader = [](Ref<Shader> shader,
                                                                                  const glm::mat4& camViewProjMat)
    {
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camViewProjMat);
      shader->Unbind();
    };
    
    // Max element and vertex to be rendered in single batch
    uint32_t maxElement = 0;
    uint32_t maxVertices = 0;
    
    Ref<Pipeline> pipeline;
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;
    Ref<Shader> shader;
    
    /// Common Batch Data Destructor
    virtual ~CommonBatchData()
    {
      pipeline.reset();
      vertexBuffer.reset();
      indexBuffer.reset();
      shader.reset();
    }
  };
  
  /// This structure holds the common batch renderer data for Quads and circle
  struct Shape2DCommonData : CommonBatchData
  {
    /// Single vertex for a Quad or Circle
    struct CommonVertex
    {
      glm::vec3 position;
      glm::vec4 color;
      glm::vec2 textureCoords;
      
      float textureIndex;
      float tilingFactor;
    };
    
    // Constants
    static constexpr uint32_t VertexForSingleElement = 4;
    static constexpr uint32_t IndicesForSingleElement = 6;
    
    /// Store the Vertex and Indices size
    uint32_t maxIndices = 0;
    /// Count of Indices to be renderer in Single Batch
    uint32_t indexCount = 0;
    
    /// Stores all the 16 Texture in array so that there is no need to load texture each frame
    /// NOTE: Load only if new texture is added or older replaced with new
    std::array<Ref<Texture>, MaxTextureSlotsInShader> textureSlots;
    
    /// Texture Slot index sent to Shader to render a specific Texture from slots
    /// NOTE: Slot 0 is reserved for white texture (No Image only color)
    uint32_t textureSlotIndex = 1; // 0 = white texture
    
    /// Basic vertex of quad (Vertex of circle is taken as Quad only)
    glm::vec4 vertexBasePosition[4];
    
    // Member Functions
    /// Initialise common shape data
    /// - Parameter maxElement: max elements
    void Init(uint32_t maxElement)
    {
      // Initialise the common data for all batch
      CommonBatchData::maxElement = maxElement;
      maxVertices = maxElement * VertexForSingleElement;
      
      // Update the Max Indices for each Batch
      maxIndices = CommonBatchData::maxElement * IndicesForSingleElement;
      
      // Creating white texture for colorful quads witout any image file
      if (!textureSlots[0])
      {
        // White data
        uint32_t whiteTextureData = 0xffffffff;
        
        // Texture specificaion
        Texture::Specification textureSpec;
        textureSpec.type = TextureType::Texture2D;
        textureSpec.internalFormat = TextureFormat::RGBA8;
        textureSpec.dataFormat = TextureFormat::RGBA;
        textureSpec.wrap = TextureWrap::Clamp;
        textureSpec.filter = TextureFilter::Linear;
        textureSpec.width = 1;
        textureSpec.height = 1;
        textureSpec.data = &whiteTextureData;
        textureSpec.size = sizeof(uint32_t);
        
        // Create white texture
        textureSlots[0] = Texture::Create(textureSpec);
      }
      
      // Setting basic Vertex point of quad
      vertexBasePosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
      vertexBasePosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
    }
    
    /// This function starts the common shape batch
    /// - Parameter camViewProjMat: camera view projection matrix
    void StartCommonBatch(const glm::mat4& camViewProjMat)
    {
      loadCamToShader(shader, camViewProjMat);
      StartCommonBatchWithoutCam();
    }
    
    /// This is common batch init
    void StartCommonBatchWithoutCam()
    {
      indexCount = 0;
      textureSlotIndex = 1;
    }
    
    // This funciton flush the shape batch
    void Flush() {
      shader->Bind();
      
      for (uint32_t i = 0; i < textureSlotIndex; i++)
      {
        textureSlots[i]->Bind(i);
      }
      
      // Render the Scene
      Renderer::DrawIndexed(pipeline, indexCount);
    }
    
    /// Shape 2D Destructor
    virtual ~Shape2DCommonData()
    {
      for(auto texture : textureSlots)
      {
        texture.reset();
      }
    }
  };

  void Renderer2D::Initialise()
  {
    s_fullscreenQuadData = CreateScope<FullScreenQuad>();
  }
  
  void Renderer2D::Shutdown()
  {
    s_fullscreenQuadData.reset();
  }
  
  void Renderer2D::DrawFullscreenQuad(const Ref<Image>& image, uint32_t slot, bool overrideShader)
  {
    // Bind the default Shader
    if (!overrideShader)
    {
      s_fullscreenQuadData->pipeline->GetSpecification().shader->Bind();
    }
    
    if (image)
    {
      image->Bind(slot);
    }
    else
    {
      s_fullscreenQuadData->whiteTexture->Bind();
    }
    Renderer::DrawQuad(s_fullscreenQuadData->pipeline);
    
    // Unbind the default Shader
    if (!overrideShader)
    {
      s_fullscreenQuadData->pipeline->GetSpecification().shader->Unbind();
    }
  }
  
  void Renderer2D::DrawFullscreenQuad(const Ref<Texture>& texture, uint32_t slot, bool overrideShader)
  {
    // Bind the default Shader
    if (!overrideShader)
    {
      s_fullscreenQuadData->pipeline->GetSpecification().shader->Bind();
    }
    
    if (texture)
    {
      texture->Bind(slot);
    }
    else
    {
      s_fullscreenQuadData->whiteTexture->Bind();
    }
    Renderer::DrawQuad(s_fullscreenQuadData->pipeline);
    
    // Unbind the default Shader
    if (!overrideShader)
    {
      s_fullscreenQuadData->pipeline->GetSpecification().shader->Unbind();
    }
  }
} // namespace IKan
