//
//  Pipeline.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.h"

namespace IKan
{
  /// This is the interface class for creating renderer Pieline
  class Pipeline
  {
  public:
    /// This is the default virtual destuctor for Pipeline
    virtual ~Pipeline() = default;
    
    /// This function binds the current Pipeline to the renderer
    virtual void Bind() const = 0;
    /// This function unbinds the current Pipeline from the renderer
    virtual void Unbind() const = 0;
    
    /// This function returns the renderer ID of Vertex Buffer
    virtual RendererID GetRendererID() const = 0;
    
    /// This function returns the specificaion reference
    virtual PipelineSpecification& GetSpecification() = 0;
    /// This function returns the const specificaion
    virtual const PipelineSpecification& GetSpecification() const = 0;
  };
} // namespace IKan
