//
//  OpenGLRendererBuffers.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include <glad/glad.h>
#include "OpenGLRendererBuffers.hpp"

namespace IKan
{
  OpenGLIndexBuffer::OpenGLIndexBuffer(void *data, uint32_t size)
  : m_size(size)
  {

  }
  
  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {

  }
  
  void OpenGLIndexBuffer::Bind() const
  {

  }
  
  void OpenGLIndexBuffer::Unbind() const
  {

  }
  
  RendererID OpenGLIndexBuffer::GetRendererID() const
  {
    return m_rendererID;
  }
  
  uint32_t OpenGLIndexBuffer::GetSize() const
  {
    return m_size;
  }
  
  uint32_t OpenGLIndexBuffer::GetCount() const
  {
    return m_size / SizeOfSingleIndices;
  }
  
} // namespace IKan
