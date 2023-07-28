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

    /// Bind the current pipeline to the GPU
    virtual void Bind() const = 0;
    /// Unbind the current pipeline to the GPU
    virtual void Unbind() const = 0;
    
    /// This function returns the renderer ID of Vertex Buffer
    virtual RendererID GetRendererID() const = 0;
    
    /// This function returns the specificaion reference
    virtual Specification& GetSpecification() = 0;
    /// This function returns the const specificaion
    virtual const Specification& GetSpecification() const = 0;

    /// This function creates the instance of Pipeline based on the renderer
    /// - Parameter spec: Pipeline specificaions
    [[nodiscard]] static Ref<Pipeline> Create(const Specification& spec);
  };
} // namespace IKan
