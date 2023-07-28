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
    
    /// This is the virtual default destructor for Pipeline
    virtual ~Pipeline() = default;

    /// This function creates the instance of Pipeline based on the renderer
    /// - Parameter spec: Pipeline specificaions
    [[nodiscard]] static Ref<Pipeline> Create(const Specification& spec);
  };
} // namespace IKan
