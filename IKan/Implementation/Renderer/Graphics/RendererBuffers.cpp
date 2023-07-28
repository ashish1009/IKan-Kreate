//
//  RendererBuffers.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include "RendererBuffers.hpp"

namespace IKan
{
  Ref<IndexBuffer> IndexBuffer::CreateWithSize(void *data, uint32_t size)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  Ref<IndexBuffer> IndexBuffer::CreateWithCount(void *data, uint32_t count)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
} // namespace IKan
