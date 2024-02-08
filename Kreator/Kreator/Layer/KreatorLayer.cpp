//
//  KreatorLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/02/24.
//

#include "KreatorLayer.hpp"

static Ref<Shader> shader;
static Ref<VertexBuffer> vertexBuffer;
static Ref<IndexBuffer> indexBuffer;
static Ref<Pipeline> pipeline;

namespace Kreator
{
  KreatorLayer::KreatorLayer()
  : Layer("Kreator Renderer")
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Creating Kreator Renderer Layer instance");
  }
  
  KreatorLayer::~KreatorLayer()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void KreatorLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Attaching '{0} Layer' to application", GetName());
    
    // Testing Temp Shader
    shader = ShaderLibrary::GetShader("../../../IKan/Assets/Shaders/BatchQuadShader.glsl");
    
    float v[] = {
      -0.5, -0.5, 0.0,
       0.5, -0.5, 0.0,
       0.5,  0.5, 0.0,
      -0.5,  0.5, 0.0,
    };
    
    uint32_t i[] = {0, 1, 2, 2, 3, 0};
    
    vertexBuffer = VertexBufferFactory::Create(v, sizeof(v));
    
    PipelineSpecification pipelineSpec;
    pipelineSpec.debugName = "Test Renderer";
    pipelineSpec.vertexLayout = {
      {"a_Position", ShaderDataType::Float3}
    };
    
    pipeline = PipelineFactory::Create(pipelineSpec);
    
    indexBuffer = IndexBufferFactory::CreateWithCount(i, 6);
  }
  void KreatorLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_INFO("Kreator Layer", "Detaching '{0} Layer' from application", GetName());
    shader.reset();
    vertexBuffer.reset();
    indexBuffer.reset();
    pipeline.reset();
  }
  
  void KreatorLayer::OnUpdate(TimeStep ts)
  {
    IK_PERFORMANCE("RendererLayer::OnUpdate");
    Renderer::Clear({0.2f, 0.22f, 0.222f, 1.0f});
    
    shader->Bind();
    shader->SetUniformMat4("u_ViewProjection", glm::mat4(1.0f));
    Renderer::DrawIndexed(pipeline, 6);
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
