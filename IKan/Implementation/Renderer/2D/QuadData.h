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
} // namespace IKan
