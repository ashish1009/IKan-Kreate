//
//  Pipeline.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Graphics/Shader.hpp"
#include "Renderer/Graphics/RendererBuffers.hpp"
#include "Renderer/Renderer.hpp"

namespace IKan
{
  /// This is the interface class to store the renderer buffers data in pipeline
  /// - Note: Create pipeline after Creating the Renderer Vertex Buffer
  class Pipeline
  {
  public:
    /// This structures stores the Pipeline specificaion
    struct Specification
    {
      std::string debugName;
      Ref<Shader> shader;
      BufferLayout layout;
    };    
  };
} // namespace IKan
