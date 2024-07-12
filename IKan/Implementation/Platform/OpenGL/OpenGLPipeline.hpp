//
//  OpenGLPipeline.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"

namespace IKan
{
  /// This class implements the APIs for creating and storing pipeline data.
  class OpenGLPipeline : public Pipeline
  {
  public:
    /// This consturctor creates the pipeline with the specification.
    /// - Parameter spec: Pipeline specificaion.
    OpenGLPipeline(const PipelineSpecification& spec);
    /// This destructior destroys the pipeline data.
    ~OpenGLPipeline();

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLPipeline);

  private:
    RendererID m_rendererID {0};
    PipelineSpecification m_specification;
  };
} // namespace IKan
