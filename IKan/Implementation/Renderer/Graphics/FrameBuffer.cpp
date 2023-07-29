//
//  FrameBuffer.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "FrameBuffer.hpp"
#include "Platform/OpenGL/OpenGLFrameBuffer.hpp"

namespace IKan
{
  FrameBuffer::Attachments::Attachments(std::initializer_list<TextureFormat> attachments)
  : textureFormats(attachments)
  {
    
  }
  
  Ref<FrameBuffer> FrameBuffer::Create(const FrameBuffer::Specification& spec)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLFrameBuffer>(spec);
      case Renderer::Api::None:
      default: IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
} // namespace IKan
