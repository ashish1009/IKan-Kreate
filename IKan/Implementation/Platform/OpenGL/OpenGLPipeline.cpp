//
//  OpenGLPipeline.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include <glad/glad.h>
#include "OpenGLPipeline.hpp"

namespace IKan
{
#define PIPELINE_LOG(...) IK_CORE_DEBUG(LogModule::Pipeline, __VA_ARGS__);
  
  OpenGLPipeline::OpenGLPipeline(const Specification& spec)
  : m_specification(spec)
  {
  }
  
  OpenGLPipeline::~OpenGLPipeline()
  {

  }  
} // namespace IKan
