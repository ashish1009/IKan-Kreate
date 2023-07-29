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
      
      IK_LOG_TRACE(LogModule::Renderer, "Initialized Fullscreen Quad Data ");
      IK_LOG_TRACE(LogModule::Renderer, "-----------------------------------------");
      IK_LOG_TRACE(LogModule::Renderer, "  Vertex Buffer used               {0} B", sizeof(vertices));
      IK_LOG_TRACE(LogModule::Renderer, "  Shader used                      {0}", pipelineSpec.shader->GetName());
    }
  };
  static Scope<FullScreenQuad> s_fullscreenQuadData;
  
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
//      s_fullscreenQuadData->whiteTexture->Bind();
    }
    Renderer::DrawQuad(s_fullscreenQuadData->pipeline);
    
    // Unbind the default Shader
    if (!overrideShader)
    {
      s_fullscreenQuadData->pipeline->GetSpecification().shader->Unbind();
    }
  }
} // namespace IKan
