//
//  Pipeline.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "Pipeline.hpp"
#include "Platform/OpenGL/OpenGLPipeline.hpp"

namespace IKan
{
  Ref<Pipeline> Pipeline::Create(const Specification &spec)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLPipeline>(spec);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
} // namespace IKan
