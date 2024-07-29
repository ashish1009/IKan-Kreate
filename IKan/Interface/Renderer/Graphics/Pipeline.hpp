//
//  Pipeline.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  /// This class is the interface for creating pipeline for renderer
  class Pipeline
  {
  public:
    /// Default virtual destructor
    virtual ~Pipeline() = default;
    
    /// This function binds the current Pipeline to the renderer
    virtual void Bind() const = 0;
    /// This function unbinds the current Pipeline from the renderer
    virtual void Unbind() const = 0;
    
    /// This function returns the renderer ID of Vertex Buffer
    virtual RendererID GetRendererID() const = 0;
    
    /// This function returns the const specification
    virtual const PipelineSpecification& GetSpecification() const = 0;
  };
} // namespace IKan
