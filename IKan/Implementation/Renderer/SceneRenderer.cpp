//
//  SceneRenderer.cpp
//  IKan
//
//  Created by Ashish . on 11/08/23.
//

#include "SceneRenderer.hpp"

namespace IKan
{
  SceneRenderer::SceneRenderer(Ref<Scene> scene)
  : m_scene(scene)
  {
    Initialize();
  }
  
  SceneRenderer::~SceneRenderer()
  {
    
  }
  
  void SceneRenderer::Initialize()
  {
    // Create Pipeline specification
    Pipeline::Specification geomatryPipelineSpec;
    geomatryPipelineSpec.debugName = "PBR-Static";
    geomatryPipelineSpec.shader = Shader::Create(CoreAssetPath("Shaders/PBR_StaticShader.glsl"));
    geomatryPipelineSpec.layout =
    {
      { "a_Position",  ShaderDataType::Float3 },
      { "a_Normal",    ShaderDataType::Float3 },
      { "a_Tangent",   ShaderDataType::Float3 },
      { "a_Bitangent", ShaderDataType::Float3 },
      { "a_TexCoord",  ShaderDataType::Float2 },
    };
    
    // Create the Pipeline instnace for full screen quad
    m_geometryPipeline = Pipeline::Create(geomatryPipelineSpec);
  }
} // namespace IKan
