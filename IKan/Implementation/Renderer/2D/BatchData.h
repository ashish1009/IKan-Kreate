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
      maxElement = 0;
      maxVertices = 0;
      
      pipeline.reset();
      vertexBuffer.reset();
      indexBuffer.reset();
      shader.reset();
    }
  };

} // namespace IKan
