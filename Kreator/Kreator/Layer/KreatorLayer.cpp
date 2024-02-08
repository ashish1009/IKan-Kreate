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
static Ref<Texture> texture;

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
      -0.5, -0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0,
       0.5, -0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0,
       0.5,  0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0,
      -0.5,  0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0,
    };
    
    uint32_t i[] = {0, 1, 2, 2, 3, 0};
    
    vertexBuffer = VertexBufferFactory::Create(v, sizeof(v));
    
    PipelineSpecification pipelineSpec;
    pipelineSpec.debugName = "Test Renderer";
    pipelineSpec.vertexLayout = 
    {
      {"a_Position",      ShaderDataType::Float3},
      {"a_Color",         ShaderDataType::Float4},
      {"a_TexCoords",     ShaderDataType::Float2},
      {"a_TexIndex",      ShaderDataType::Float},
      {"a_TilingFactor",  ShaderDataType::Float}
    };
    
    pipeline = PipelineFactory::Create(pipelineSpec);
    
    indexBuffer = IndexBufferFactory::CreateWithCount(i, 6);
    
    texture = TextureFactory::Create("/Users/ashish./iKan_storage/Github/Projects/ikan_ws_recent_ecs_ray/kreator/editor/editor_assets/texture/basicTextures/checkerboard.png");
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
    shader->SetUniformMat4("u_ViewProjection", Utils::Math::UnitMat4);
    texture->Bind();
    Renderer::DrawIndexed(pipeline, 6);
  }
  
  void KreatorLayer::OnEvent(Event& event)
  {
    
  }
  
  void KreatorLayer::OnImGuiRender()
  {
    
  }
} // namespace Kreator
