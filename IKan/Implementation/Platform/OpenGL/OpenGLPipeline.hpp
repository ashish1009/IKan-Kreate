//
//  OpenGLPipeline.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Graphics/Pipeline.hpp"

namespace IKan
{
  /// This is the Open GL implementation class for Pipeline
  class OpenGLPipeline : public Pipeline
  {
  public:
    /// This constructor creates the Open GL Pipeline instace
    /// - Parameter spec: Pipeline specificaion
    OpenGLPipeline(const Specification& spec);
    /// This is the Open GL Pipeline destructor
    ~OpenGLPipeline();
        
  private:
    /// This function invalidates the pipeline with new specificaion
    void Invalidate();
    
    RendererID m_rendererID;
    Specification m_specification;
  };
} // namespace IKan
