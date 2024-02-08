//
//  OpenGLPipeline.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"

namespace IKan
{
  /// Interface class for storing Pipeline
  class OpenGLPipeline : public Pipeline
  {
  public:
    /// This is the consturctor of frame buffer the create the buffer with the specification
    /// - Parameter spec: Pipeline specificaion
    OpenGLPipeline(const PipelineSpecification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLPipeline();
    
    /// This function binds the current Pipeline to the renderer
    void Bind() const override;
    /// This function unbinds the current Pipeline from the renderer
    void Unbind() const override;
    
    /// This function returns the renderer ID of Vertex Buffer
    RendererID GetRendererID() const override;
    
    /// This function returns the specificaion reference
    PipelineSpecification& GetSpecification() override;
    /// This function returns the const specificaion
    const PipelineSpecification& GetSpecification() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLPipeline);
    
  private:
    /// This function invalidates the pipeline with new specificaion
    void Invalidate();
    /// This function adds the single attribute
    /// - Parameter layout: layout data
    void AddAttrib(const BufferLayout& layout);
    
    RendererID m_rendererID;
    PipelineSpecification m_specification;
  };
} // namespace IKan
