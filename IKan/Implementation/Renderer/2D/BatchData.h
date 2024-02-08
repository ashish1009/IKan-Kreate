//
//  BatchData.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"
#include "Renderer/Graphics/RendererBuffer.hpp"
#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  struct CommonBatchData
  {
    // Lambda to load the camera data to shdaer
    const std::function<void(Ref<Shader>, const glm::mat4&)> LoadCamToShader = [](Ref<Shader> shader, const glm::mat4& camViewProjMat)
    {
      IK_PERFORMANCE("Bacth::LoadShaderData");
      shader->Bind();
      shader->SetUniformMat4("u_ViewProjection", camViewProjMat);
      shader->Unbind();
    };
    
    // Max element and vertex to be rendered in single batch
    uint32_t maxElement {0};
    uint32_t maxVertices {0};
    
    // Graphics Data
    Ref<Pipeline> pipeline;
    Ref<VertexBuffer> vertexBuffer;
    Ref<IndexBuffer> indexBuffer;
    Ref<Shader> shader;
    
    void Destroy()
    {
      IK_PROFILE();
      maxElement = 0;
      maxVertices = 0;
      
      pipeline.reset();
      vertexBuffer.reset();
      indexBuffer.reset();
      shader.reset();
    }
  };

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
      
      int32_t pixelID;
    };
    
    // Constants
    static constexpr uint32_t VertexForSingleElement = 4;
    static constexpr uint32_t IndicesForSingleElement = 6;
    
    /// Store the Vertex and Indices size
    uint32_t maxIndices = 0;
    /// Count of Indices to be renderer in Single Batch
    uint32_t indexCount = 0;
    
    /// Texture Slot index sent to Shader to render a specific Texture from slots
    /// NOTE: Slot 0 is reserved for white texture (No Image only color)
    uint32_t textureSlotIndex = 1; // 0 = white texture
    
    /// Stores all the 16 Texture in array so that there is no need to load texture each frame
    /// NOTE: Load only if new texture is added or older replaced with new
    std::array<Ref<Texture>, MaxTextureSlotsInShader> textureSlots;
    
    /// Basic vertex of quad (Vertex of circle is taken as Quad only)
    glm::vec4 vertexBasePosition[4];
    
    // Member Functions
    void InitializeShapeData(uint32_t maxElement)
    {
      IK_PROFILE();
      CommonBatchData::maxElement = maxElement;
      maxVertices = maxElement * VertexForSingleElement;
      maxIndices = CommonBatchData::maxElement * IndicesForSingleElement;
      
      // Setting basic Vertex point of quad
      vertexBasePosition[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
      vertexBasePosition[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
      vertexBasePosition[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
      
      // Creating white texture for colorful (default white) quads witout any image file
      if (!textureSlots[0])
      {
        textureSlots[0] = TextureFactory::Create(0xffffffff);
      }
    }
    
    void Destroy()
    {
      IK_PROFILE();
      maxIndices = 0;
      for (uint32_t i = 0; i < MaxTextureSlotsInShader; i++)
      {
        textureSlots[i].reset();
      }
      CommonBatchData::Destroy();
    }
  };
} // namespace IKan
