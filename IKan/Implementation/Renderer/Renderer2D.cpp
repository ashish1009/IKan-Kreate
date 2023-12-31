//
//  Renderer2D.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Renderer2D.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/TextRenderer.hpp"
#include "Renderer/UI/Font.hpp"
#include "Renderer/RendererStats.hpp"
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
      // Create vertes Buffer
      vertexBuffer = VertexBuffer::Create((void*)&vertices, sizeof(vertices));
      
      // Create Pipeline specification
      Pipeline::Specification pipelineSpec;
      pipelineSpec.debugName = "Full Screen Quad Renderer";
      pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/FSQuadShader.glsl"));
      pipelineSpec.vertexLayout =
      {
        { "a_Position", ShaderDataType::Float3 },
        { "a_TexCoord", ShaderDataType::Float2 },
      };
      
      // Create the Pipeline instnace for full screen quad
      pipeline = Pipeline::Create(pipelineSpec);
      
      // Create white texture
      whiteTexture = Texture::CreateWhiteTexture();
      
      IK_LOG_TRACE(LogModule::Renderer2D, "Initialized Fullscreen Quad Data ");
      IK_LOG_TRACE(LogModule::Renderer2D, "-----------------------------------------");
      IK_LOG_TRACE(LogModule::Renderer2D, "  Vertex Buffer used               {0} B", sizeof(vertices));
      IK_LOG_TRACE(LogModule::Renderer2D, "  Shader used                      {0}", pipelineSpec.shader->GetName());
    }
  };
  static Scope<FullScreenQuad> s_fullscreenQuadData;
  
#define BATCH_INFO(...) IK_LOG_INFO(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_TRACE(...) IK_LOG_TRACE(LogModule::Renderer2D, __VA_ARGS__)
#define BATCH_WARN(...) IK_LOG_WARN(LogModule::Renderer2D, __VA_ARGS__)
  
  static constexpr glm::vec2 TextureCoords[] =
  {
    { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
  };
  
  /// This structure stores the environmant data
  struct Environment
  {
    glm::mat4 cameraViewProjectionMatrix;
    glm::mat4 cameraViewMatrix;
  };
  
  /// Store the Environment data
  static Environment s_environment;
  
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
    /// Initialize common shape data
    /// - Parameter maxElement: max elements
    void Inititialize(uint32_t maxElement)
    {
      // Initialize the common data for all batch
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
        TextureSpecification textureSpec;
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

  /// Batch Data to Rendering Quads
  struct QuadData : Shape2DCommonData
  {
    /// Single vertex of a Quad
    struct Vertex : CommonVertex
    {
      int32_t pixelID;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    // Member functions
    /// This function starts the batch for Lines
    /// - Parameter camViewProjMat: camera projection matrix
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      StartCommonBatch(camViewProjMat);
      StartInternalBatch();
    }
    
    void StartInternalBatch()
    {
      StartCommonBatchWithoutCam();
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    /// Quad Data Destructor
    virtual ~QuadData()
    {
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  static Scope<QuadData> s_quadData;

  /// Batch Data to Rendering Circles
  struct CircleData : Shape2DCommonData
  {
    /// Single vertex of a Circle
    struct Vertex : CommonVertex
    {
      glm::vec3 localPosition;
      float thickness;
      float fade;
      
      int32_t pixelID;
    };
    
    /// Base pointer of Vertex Data. This is start of Batch data for single draw call
    Vertex* vertexBufferBasePtr = nullptr;
    /// Incrememntal Vetrtex Data Pointer to store all the batch data in Buffer
    Vertex* vertexBufferPtr = nullptr;
    
    // Member functions
    /// This function starts the batch for Lines
    /// - Parameter camViewProjMat: camera projection matrix
    void StartBatch(const glm::mat4& camViewProjMat)
    {
      StartCommonBatch(camViewProjMat);
      StartInternalBatch();
    }
    
    void StartInternalBatch()
    {
      StartCommonBatchWithoutCam();
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    /// Circle Data Destructor
    virtual ~CircleData()
    {
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  static Scope<CircleData> s_circleData;
  
  /// Batch Data to Rendering Lines
  struct LineData : CommonBatchData
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
    
    // Member function
    /// This function starts the batch for Lines
    /// - Parameter camViewProjMat: camera projection matrix
    void StartBatch(const glm::mat4& camViewProjMat) {
      loadCamToShader(shader, camViewProjMat);
      StartInternalBatch();
    }
    
    void StartInternalBatch()
    {
      vertexCount = 0;
      vertexBufferPtr = vertexBufferBasePtr;
    }
    
    /// Liene Data Desdtructor
    virtual ~LineData()
    {
      ikdelete [] vertexBufferBasePtr;
      vertexBufferBasePtr = nullptr;
    }
  };
  static Scope<LineData> s_lineData;
  
  void Renderer2D::Initialize(const Renderer2DData& renderer2DData)
  {
    IK_PROFILE();
    s_fullscreenQuadData = CreateScope<FullScreenQuad>();

    BATCH_INFO("Initialising the Batch Renderer 2D ");
    BATCH_INFO("-----------------------------------");
    
    AddQuadData(renderer2DData.maxQuads);
    AddCircleData(renderer2DData.maxCircles);
    AddLineData(renderer2DData.maxLines);
  }
  
  void Renderer2D::Shutdown()
  {
    IK_PROFILE();
    BATCH_WARN("Shutting Down the Batch Renderer 2D ");
    BATCH_WARN("------------------------------------");
    
    if (s_quadData)
    {
      BATCH_WARN("Destrying Batch Renderer for Quad Data ");
      BATCH_WARN("-----------------------------------------");
      BATCH_WARN("  Max Quads per Batch              {0}", s_quadData->maxElement);
      BATCH_WARN("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
      BATCH_WARN("  Vertex Buffer used               {0} B", s_quadData->maxVertices * sizeof(QuadData::Vertex));
      BATCH_WARN("  Index Buffer used                {0} B", s_quadData->maxIndices * sizeof(uint32_t));
      BATCH_WARN("  Shader Used                      {0}", s_quadData->shader->GetName());
      
      RendererStatistics::Get()._2d.maxQuads = s_quadData->maxElement;
      
      s_quadData.reset();
    }
    
    if (s_circleData)
    {
      BATCH_WARN("Destroying Batch Renderer for Circle Data ");
      BATCH_WARN("-------------------------------------------");
      BATCH_WARN("  Max Circles per Batch            {0}", s_circleData->maxElement);
      BATCH_WARN("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
      BATCH_WARN("  Vertex Buffer used               {0} B", s_circleData->maxVertices * sizeof(CircleData::Vertex));
      BATCH_WARN("  Index Buffer used                {0} B", s_circleData->maxIndices * sizeof(uint32_t));
      BATCH_WARN("  Shader Used                      {0}", s_circleData->shader->GetName());
      
      RendererStatistics::Get()._2d.maxCircles = s_circleData->maxElement;
      
      s_circleData.reset();
    }
    
    if (s_lineData)
    {
      BATCH_WARN("Destroying Batch Renderer for Line Data ");
      BATCH_WARN("-----------------------------------------");
      BATCH_WARN("  Max Lines per Batch              {0}", s_lineData->maxElement);
      BATCH_WARN("  Vertex Buffer used               {0} B", s_lineData->maxVertices * sizeof(LineData::Vertex));
      BATCH_WARN("  Shader Used                      {0}", s_lineData->shader->GetName());
      
      RendererStatistics::Get()._2d.maxLines = s_lineData->maxElement;
      
      s_lineData.reset();
    }

    // Destroy Full screen data
    s_fullscreenQuadData.reset();
  }
  
  void Renderer2D::AddQuadData(uint32_t maxElement)
  {
    if (maxElement == 0)
    {
      return;
    }
    
    Scope<QuadData>& data = s_quadData;
    
    // If data have already created then append the data to previous one
    if (data)
    {
      maxElement += data->maxElement;
      data.reset();
    }
    
    // Allocate memory for Quad Data
    data = CreateScope<QuadData>();
    
    // Initialize the data for Common shape
    data->Inititialize(maxElement);
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertexBufferBasePtr = new QuadData::Vertex[data->maxVertices];
    
    // Create vertes Buffer
    data->vertexBuffer = VertexBuffer::Create(data->maxVertices * sizeof(QuadData::Vertex));
    
    // Create Pipeline specification
    Pipeline::Specification pipelineSpec;
    pipelineSpec.debugName = "Quad Renderer";
    pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/QuadShader.glsl"));
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
    data->pipeline = Pipeline::Create(pipelineSpec);
    
    // Create Index Buffer
    uint32_t* indices = iknew uint32_t[data->maxIndices];
    uint32_t offset = 0;
    for (size_t i = 0; i < data->maxIndices; i += Shape2DCommonData::IndicesForSingleElement)
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
    data->indexBuffer = IndexBuffer::CreateWithCount(indices, data->maxIndices);
    ikdelete[] indices;
    
    // Setup the Quad Shader. Copy the Same shader refernce which is stored in the Pipeline
    data->shader = data->pipeline->GetSpecification().shader;
    
    RendererStatistics::Get()._2d.maxQuads = data->maxElement;
    
    BATCH_INFO("Initialized Batch Renderer for Quad Data ");
    BATCH_INFO("-----------------------------------------");
    BATCH_INFO("  Max Quads per Batch              {0}", data->maxElement);
    BATCH_INFO("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
    BATCH_INFO("  Vertex Buffer used               {0} B", data->maxVertices * sizeof(QuadData::Vertex));
    BATCH_INFO("  Index Buffer used                {0} B", data->maxIndices * sizeof(uint32_t));
    BATCH_INFO("  Shader Used                      {0}", data->shader->GetName());
  }

  void Renderer2D::AddCircleData(uint32_t maxElement)
  {
    if (maxElement == 0)
    {
      return;
    }
    Scope<CircleData>& data = s_circleData;
    
    // If data have already created then append the data to previous one
    if (data)
    {
      maxElement += data->maxElement;
      data.reset();
    }
    
    // Allocate memory for Circle Data
    data = CreateScope<CircleData>();
    
    // Initialize the data for Common shape
    data->Inititialize(maxElement);
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertexBufferBasePtr = iknew CircleData::Vertex[data->maxVertices];
    
    // Create vertes Buffer
    data->vertexBuffer = VertexBuffer::Create(data->maxVertices * sizeof(CircleData::Vertex));
    
    // Create Pipeline
    Pipeline::Specification pipelineSpec;
    pipelineSpec.debugName = "Circle Renderer";
    pipelineSpec.shader = Shader::Create (CoreAssetPath("Shaders/CircleShader.glsl"));
    pipelineSpec.vertexLayout =
    {
      { "a_Position",     ShaderDataType::Float3 },
      { "a_Color",        ShaderDataType::Float4 },
      { "a_TexCoords",    ShaderDataType::Float2 },
      { "a_TexIndex",     ShaderDataType::Float },
      { "a_TilingFactor", ShaderDataType::Float },
      { "a_LocalPosition",ShaderDataType::Float3 },
      { "a_Thickness",    ShaderDataType::Float },
      { "a_Fade",         ShaderDataType::Float },
      { "a_ObjectID",     ShaderDataType::Int },
    };
    
    // Create the Pipeline instnace
    data->pipeline = Pipeline::Create(pipelineSpec);
    
    // Create Index Buffer
    uint32_t* indices = iknew uint32_t[data->maxIndices];
    uint32_t offset = 0;
    for (size_t i = 0; i < data->maxIndices; i += Shape2DCommonData::IndicesForSingleElement)
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
    data->indexBuffer = IndexBuffer::CreateWithCount(indices, data->maxIndices);
    ikdelete[] indices;
    
    // Setup the Quad Shader
    data->shader = data->pipeline->GetSpecification().shader;
    
    RendererStatistics::Get()._2d.maxCircles = data->maxElement;
    
    BATCH_INFO("Initialized Batch Renderer for Circle Data ");
    BATCH_INFO("-------------------------------------------");
    BATCH_INFO("  Max Circles per Batch            {0}", data->maxElement);
    BATCH_INFO("  Max Texture Slots per Batch      {0}", MaxTextureSlotsInShader);
    BATCH_INFO("  Vertex Buffer used               {0} B", data->maxVertices * sizeof(CircleData::Vertex));
    BATCH_INFO("  Index Buffer used                {0} B", data->maxIndices * sizeof(uint32_t));
    BATCH_INFO("  Shader Used                      {0}", data->shader->GetName());
  }
  
  void Renderer2D::AddLineData(uint32_t maxElement)
  {
    if (maxElement == 0)
    {
      return;
    }
    
    Scope<LineData>& data = s_lineData;
    
    // If data have already created then append the data to previous one
    if (data)
    {
      maxElement += data->maxElement;
      data.reset();
    }
    
    // Allocate memory for Line Data
    data = CreateScope<LineData>();
    
    // Initialize the data for Common shape
    data->maxElement = maxElement;
    data->maxVertices = data->maxElement * LineData::VertexForSingleLine;
    
    // Allocating the memory for vertex Buffer Pointer
    data->vertexBufferBasePtr = iknew LineData::Vertex[data->maxVertices];
    
    // Create vertes Buffer
    data->vertexBuffer = VertexBuffer::Create(data->maxVertices * sizeof(LineData::Vertex));
    
    // Create Pipeline
    Pipeline::Specification pipelineSpec;
    pipelineSpec.debugName = "Line Renderer";
    pipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/LineShader.glsl"));
    pipelineSpec.vertexLayout =
    {
      { "a_Position", ShaderDataType::Float3 },
      { "a_Color",    ShaderDataType::Float4 },
    };
    
    // Create the Pipeline instnace
    data->pipeline = Pipeline::Create(pipelineSpec);
    
    // Setup the Quad Shader
    data->shader = data->pipeline->GetSpecification().shader;
    
    RendererStatistics::Get()._2d.maxLines = data->maxElement;
    
    BATCH_INFO("Initialized Batch Renderer for Line Data ");
    BATCH_INFO("-----------------------------------------");
    BATCH_INFO("  Max Lines per Batch              {0}", data->maxElement);
    BATCH_INFO("  Vertex Buffer used               {0} B", data->maxVertices * sizeof(LineData::Vertex));
    BATCH_INFO("  Shader Used                      {0}", data->shader->GetName());
  }
    
  void Renderer2D::BeginBatch(const glm::mat4 &camViewProjMat, const glm::mat4 &cameraViewMat)
  {
    // Start batch for quads
    s_environment.cameraViewProjectionMatrix = camViewProjMat;
    s_environment.cameraViewMatrix = cameraViewMat;

    if (s_quadData)
    {
      s_quadData->StartBatch(camViewProjMat);
    }
    
    if (s_circleData)
    {
      s_circleData->StartBatch(camViewProjMat);
    }
    
    if (s_lineData)
    {
      s_lineData->StartBatch(camViewProjMat);
    }
    
    TextRenderer::BeginBatch(camViewProjMat);
  }
  
  void Renderer2D::EndBatch()
  {
    Flush();
    TextRenderer::EndBatch();
  }

  void Renderer2D::Flush()
  {
    if (s_quadData and s_quadData->indexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_quadData->vertexBufferPtr - (uint8_t*)s_quadData->vertexBufferBasePtr);
      s_quadData->vertexBuffer->SetData(s_quadData->vertexBufferBasePtr, dataSize);
      s_quadData->Flush();
    }
    
    if (s_circleData and s_circleData->indexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_circleData->vertexBufferPtr - (uint8_t*)s_circleData->vertexBufferBasePtr);
      s_circleData->vertexBuffer->SetData(s_circleData->vertexBufferBasePtr, dataSize);
      s_circleData->Flush();
    }
    
    if (s_lineData and s_lineData->vertexCount)
    {
      uint32_t dataSize = (uint32_t)((uint8_t*)s_lineData->vertexBufferPtr - (uint8_t*)s_lineData->vertexBufferBasePtr);
      s_lineData->vertexBuffer->SetData(s_lineData->vertexBufferBasePtr, dataSize);
      
      s_lineData->shader->Bind();
      // Render the Scene
      Renderer::DrawLines(s_lineData->pipeline, s_lineData->vertexCount);
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
  
  void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int32_t objectID)
  {
    DrawTextureQuad(transform, nullptr, TextureCoords, 1.0f /* tiling factor */, color, objectID);
  }
  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation,
                            const glm::vec4& color, int32_t objectID)
  {
    auto transform  = Utils::Math::GetTransformMatrix(position, rotation, scale);
    DrawTextureQuad(transform, nullptr, TextureCoords, 1.0f /* tiling factor */, color, objectID);
  }

  void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation,
                            const Ref<Texture>& texture, const glm::vec4& color, int32_t objectID)
  {
    auto transform  = Utils::Math::GetTransformMatrix(position, rotation, scale);
    DrawTextureQuad(transform, texture, TextureCoords, 1.0f /* tiling factor */, color, objectID);
  }
  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,
                            float tilingFactor, int32_t objectID)
  {
    DrawTextureQuad(transform, texture ? texture : nullptr, TextureCoords, tilingFactor, tintColor, objectID );
  }
  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Sprite>& subTexture, const glm::vec4& tintColor,
                            int32_t objectID)
  {
    const glm::vec2* textureCoords = subTexture->GetTexCoord();
    DrawTextureQuad(transform, (subTexture) ? subTexture->GetImage() : nullptr,
                    (subTexture) ? textureCoords : TextureCoords, 1.0f, tintColor, objectID);
  }
  
  void Renderer2D::DrawTextureQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec2* textureCoords,
                                   float tilingFactor, const glm::vec4& tintColor, int32_t objectID)
  {
    // If number of indices increase in batch then start new batch
    if (s_quadData->indexCount >= s_quadData->maxIndices)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_quadData->indexCount);
      EndBatch();
      s_quadData->StartInternalBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_quadData->textureSlotIndex; i++)
      {
        if (s_quadData->textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_quadData->textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_quadData->textureSlotIndex);
          EndBatch();
          s_quadData->StartInternalBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_quadData->textureSlotIndex;
        s_quadData->textureSlots[s_quadData->textureSlotIndex] = texture;
        s_quadData->textureSlotIndex++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++)
    {
      s_quadData->vertexBufferPtr->position        = transform * s_quadData->vertexBasePosition[i];
      s_quadData->vertexBufferPtr->color           = tintColor;
      s_quadData->vertexBufferPtr->textureCoords   = textureCoords[i];
      s_quadData->vertexBufferPtr->textureIndex    = textureIndex;
      s_quadData->vertexBufferPtr->tilingFactor    = tilingFactor;
      s_quadData->vertexBufferPtr->pixelID         = objectID;
      s_quadData->vertexBufferPtr++;
    }
    
    s_quadData->indexCount += Shape2DCommonData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }
  
  void Renderer2D::DrawFixedViewQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,
                                     float tilingFactor, int32_t entityID)
  {
    glm::vec3 position, rotation, scale;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    DrawFixedViewQuad(position, scale, texture, tintColor, tilingFactor, entityID);
  }
  
  void Renderer2D::DrawFixedViewQuad(const glm::vec3& position, const glm::vec3& scale, const Ref<Texture>& texture,
                                        const glm::vec4& tintColor, float tilingFactor, int32_t entityID)
  {
    // If number of indices increase in batch then start new batch
    if (s_quadData->indexCount >= s_quadData->maxIndices)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_quadData->indexCount);
      EndBatch();
      s_quadData->StartInternalBatch();
    }

    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_quadData->textureSlotIndex; i++)
      {
        if (s_quadData->textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_quadData->textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_quadData->textureSlotIndex);
          EndBatch();
          s_quadData->StartInternalBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_quadData->textureSlotIndex;
        s_quadData->textureSlots[s_quadData->textureSlotIndex] = texture;
        s_quadData->textureSlotIndex++;
      }
    }
    
    // get the fixed view from camera view matrix
    glm::vec3 camRightWS =
    {
       s_environment.cameraViewMatrix[0][0],
       s_environment.cameraViewMatrix[1][0],
       s_environment.cameraViewMatrix[2][0]
    };
    
    glm::vec3 camUpWS =
    {
      s_environment.cameraViewMatrix[0][1],
      s_environment.cameraViewMatrix[1][1],
      s_environment.cameraViewMatrix[2][1]
    };
    
    for (size_t i = 0; i < QuadData::VertexForSingleElement; i++)
    {
      s_quadData->vertexBufferPtr->position         = position + camRightWS *
      s_quadData->vertexBasePosition[i].x * scale.x + camUpWS *
      s_quadData->vertexBasePosition[i].y * scale.y;
      
      s_quadData->vertexBufferPtr->color            = tintColor;
      s_quadData->vertexBufferPtr->textureCoords    = TextureCoords[i];
      s_quadData->vertexBufferPtr->textureIndex     = textureIndex;
      s_quadData->vertexBufferPtr->tilingFactor     = tilingFactor;
      s_quadData->vertexBufferPtr->pixelID          = entityID;
      s_quadData->vertexBufferPtr++;
    }
    
    s_quadData->indexCount += QuadData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }

  void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec3& radius, const glm::vec3& rotation, const glm::vec4& color,
                              float thickness, float fade, int32_t objectID)
  {
    auto transform = Utils::Math::GetTransformMatrix(position, rotation, radius);
    DrawTextureCircle(transform, nullptr, 1.0f /* tiling factor */, color, thickness, fade, objectID);
  }
  
  void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int32_t objectID)
  {
    DrawTextureCircle(transform, nullptr, 1.0f /* tiling factor */, color, thickness, fade, objectID);
  }
  void Renderer2D::DrawCircle(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,
                              float tilingFactor, float thickness, float fade, int32_t objectID)
  {
    DrawTextureCircle(transform, texture, tilingFactor, tintColor, thickness, fade, objectID);
  }
  void Renderer2D::DrawFixedViewCircle(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,
                                       float thickness, float fade, float tilingFactor, int32_t entityID)
  {
    glm::vec3 position, rotation, scale;
    Utils::Math::DecomposeTransform(transform, position, rotation, scale);
    DrawFixedViewCircle(position, scale, texture, tintColor, thickness, fade, tilingFactor, entityID);
  }
  
  void Renderer2D::DrawFixedViewCircle(const glm::vec3& position, const glm::vec3& scale, const Ref<Texture>& texture,
                                       const glm::vec4& tintColor, float thickness, float fade, float tilingFactor,
                                       int32_t entityID)
  {
    // If number of indices increase in batch then start new batch
    if (s_circleData->indexCount >= s_circleData->maxIndices)
    {
      BATCH_WARN("Starts the new batch as number of indices ({0}) increases in the previous batch", s_circleData->indexCount);
      EndBatch();
      s_circleData->StartInternalBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_circleData->textureSlotIndex; i++)
      {
        if (s_circleData->textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_circleData->textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_WARN("Starts the new batch as number of texture slot ({0}) increases in the previous batch", s_circleData->textureSlotIndex);
          EndBatch();
          s_circleData->StartInternalBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_circleData->textureSlotIndex;
        s_circleData->textureSlots[s_circleData->textureSlotIndex] = texture;
        s_circleData->textureSlotIndex++;
      }
    }
    
    // get the fixed view from camera view matrix
    glm::vec3 camRightWS =
    {
      s_environment.cameraViewMatrix[0][0],
      s_environment.cameraViewMatrix[1][0],
      s_environment.cameraViewMatrix[2][0]
    };
    
    glm::vec3 camUpWS =
    {
      s_environment.cameraViewMatrix[0][1],
      s_environment.cameraViewMatrix[1][1],
      s_environment.cameraViewMatrix[2][1]
    };
    
    for (size_t i = 0; i < CircleData::VertexForSingleElement; i++)
    {
      s_circleData->vertexBufferPtr->position         = position + camRightWS *
      s_circleData->vertexBasePosition[i].x * scale.x + camUpWS *
      s_circleData->vertexBasePosition[i].y * scale.y;
      
      s_circleData->vertexBufferPtr->color           = tintColor;
      s_circleData->vertexBufferPtr->textureCoords   = 2.0f * s_circleData->vertexBasePosition[i];
      s_circleData->vertexBufferPtr->textureIndex    = textureIndex;
      s_circleData->vertexBufferPtr->tilingFactor    = tilingFactor;
      s_circleData->vertexBufferPtr->localPosition   = 2.0f * s_circleData->vertexBasePosition[i];
      s_circleData->vertexBufferPtr->thickness       = thickness;
      s_circleData->vertexBufferPtr->fade            = fade;
      s_circleData->vertexBufferPtr->pixelID         = entityID;
      s_circleData->vertexBufferPtr++;
    }
    
    s_circleData->indexCount += CircleData::IndicesForSingleElement;
    
    // Update Stats
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.quads++;
  }
  void Renderer2D::DrawTextureCircle(const glm::mat4& transform, const Ref<Texture>& texture, float tilingFactor,
                                     const glm::vec4& tintColor, float thickness, float fade, int32_t objectID)
  {
    // If number of indices increase in batch then start new batch
    if (s_circleData->indexCount >= s_circleData->maxIndices)
    {
      BATCH_INFO("Starts the new batch as number of indices ({0}) increases in the previous batch", s_circleData->indexCount);
      EndBatch();
      s_circleData->StartInternalBatch();
    }
    
    float textureIndex = 0.0f;
    if (texture)
    {
      // Find if texture is already loaded in current batch
      for (size_t i = 1; i < s_circleData->textureSlotIndex; i++)
      {
        if (s_circleData->textureSlots[i].get() == texture.get())
        {
          // Found the current textue in the batch
          textureIndex = (float)i;
          break;
        }
      }
      
      // If current texture slot is not pre loaded then load the texture in proper slot
      if (textureIndex == 0.0f)
      {
        // If number of slots increases max then start new batch
        if (s_circleData->textureSlotIndex >= MaxTextureSlotsInShader)
        {
          BATCH_INFO("Starts the new batch as number of texture slot ({0}) increases in the previous batch",
                     s_circleData->textureSlotIndex);
          EndBatch();
          s_circleData->StartInternalBatch();
        }
        
        // Loading the current texture in the first free slot slot
        textureIndex = (float)s_circleData->textureSlotIndex;
        s_circleData->textureSlots[s_circleData->textureSlotIndex] = texture;
        s_circleData->textureSlotIndex++;
      }
    }
    
    for (size_t i = 0; i < Shape2DCommonData::VertexForSingleElement; i++)
    {
      s_circleData->vertexBufferPtr->position        = transform * s_circleData->vertexBasePosition[i];
      s_circleData->vertexBufferPtr->color           = tintColor;
      s_circleData->vertexBufferPtr->textureCoords   = 2.0f * s_circleData->vertexBasePosition[i];
      s_circleData->vertexBufferPtr->textureIndex    = textureIndex;
      s_circleData->vertexBufferPtr->tilingFactor    = tilingFactor;
      s_circleData->vertexBufferPtr->localPosition   = 2.0f * s_circleData->vertexBasePosition[i];
      s_circleData->vertexBufferPtr->thickness       = thickness;
      s_circleData->vertexBufferPtr->fade            = fade;
      s_circleData->vertexBufferPtr->pixelID         = objectID;
      s_circleData->vertexBufferPtr++;
    }
    
    s_circleData->indexCount += Shape2DCommonData::IndicesForSingleElement;
    
    RendererStatistics::Get().indexCount += Shape2DCommonData::IndicesForSingleElement;
    RendererStatistics::Get().vertexCount += Shape2DCommonData::VertexForSingleElement;
    RendererStatistics::Get()._2d.circles++;
  }
  
  void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
  {
    // If number of indices increase in batch then start new batch
    if (s_lineData->vertexCount >= s_lineData->maxVertices)
    {
      BATCH_INFO("Starts the new batch as number of vertices ({0}) increases in the previous batch", s_lineData->vertexCount);
      EndBatch();
      s_lineData->StartInternalBatch();
    }
    
    s_lineData->vertexBufferPtr->position = p0;
    s_lineData->vertexBufferPtr->color = color;
    s_lineData->vertexBufferPtr++;
    
    s_lineData->vertexBufferPtr->position = p1;
    s_lineData->vertexBufferPtr->color = color;
    s_lineData->vertexBufferPtr++;
    
    s_lineData->vertexCount += LineData::VertexForSingleLine;
    RendererStatistics::Get().vertexCount += LineData::VertexForSingleLine;
    RendererStatistics::Get()._2d.lines++;
  }
  
  void Renderer2D::DrawRect(const glm::vec3& p0, const glm::vec3& p2, const glm::vec4& color)
  {
    glm::vec3 p1 = glm::vec3(p2.x, p0.y, p0.z);
    glm::vec3 p3 = glm::vec3(p0.x, p2.y, p2.z);
    
    DrawRect(p0, p1, p2, p3, color);
  }
  
  void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
  {
    glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
    glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);
    
    DrawRect(p0, p1, p2, p3, color);
  }

  
  void Renderer2D::DrawRect(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
                            const glm::vec4& color)
  {
    DrawLine(p0, p1, color);
    DrawLine(p1, p2, color);
    DrawLine(p2, p3, color);
    DrawLine(p3, p0, color);
  }

  void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color)
  {
    glm::vec3 lineVertices[4];
    for (size_t i = 0; i < 4; i++)
    {
      lineVertices[i] = transform * s_quadData->vertexBasePosition[i];
    }
    
    DrawLine(lineVertices[0], lineVertices[1], color);
    DrawLine(lineVertices[1], lineVertices[2], color);
    DrawLine(lineVertices[2], lineVertices[3], color);
    DrawLine(lineVertices[3], lineVertices[0], color);
  }

  void Renderer2D::RenderFixedViewText(const std::string& text, Ref<Font> font, glm::vec3 position,
                                       const glm::vec2& scale, const glm::vec4& color, int32_t objectID)
  {
    TextRenderer::RenderFixedViewText(text, font, position, scale, color, objectID);
  }
  
  void Renderer2D::RenderText(const std::string& text, Ref<Font> font, glm::vec3 position, const glm::vec2& scale,
                              const glm::vec4& color, int32_t objectID)
  {
    TextRenderer::RenderText(text, font, position, scale, color, objectID);
  }
} // namespace IKan
