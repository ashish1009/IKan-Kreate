//
//  OpenGLRendererBuffer.cpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#include "OpenGLRendererBuffer.hpp"

#include <glad/glad.h>

#include "Renderer/RendererStats.hpp"

namespace IKan
{
  // Vertex Buffer ---------------------------------------------------------------------------------------------------
  OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32_t size)
  : m_size(size)
  {
    m_localData = Buffer::Copy(data, size);
    Renderer::Submit([this]() {
      IK_PROFILE();
      if (!m_localData.data or m_size == 0)
      {
        return;
      }
      
      glGenBuffers(1, &m_rendererID);
      RendererStatistics::Get().vertexBufferSize += m_size;
      
      glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
      glBufferData(GL_ARRAY_BUFFER, m_size, m_localData.data, GL_STATIC_DRAW);
      
      IK_LOG_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer with Data.");
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       | {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              | {0} B", m_size);
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Used   | {0} B", RendererStatistics::Get().vertexBufferSize);
    });
  }
  
  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
  : m_size(size)
  {
    Renderer::Submit([this, size]() {
      IK_PROFILE();
      if(size == 0)
      {
        return;
      }
      
      glGenBuffers(1, &m_rendererID);
      RendererStatistics::Get().vertexBufferSize += m_size;
      
      glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
      glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
      
      IK_LOG_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer without Data.");
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       | {0}", m_rendererID);
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              | {0} B", m_size);
      IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Used   | {0} B", RendererStatistics::Get().vertexBufferSize);
    });
  }
  
  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    IK_PROFILE();
    RendererStatistics::Get().vertexBufferSize -= m_size;
    IK_LOG_DEBUG(LogModule::VertexBuffer, "Destroying Open GL Vertex Buffer.");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       | {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              | {0} B", m_size);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Left   | {0} B", RendererStatistics::Get().vertexBufferSize);
    
    Renderer::Submit([this]() {
      glDeleteBuffers(1, &m_rendererID);
    });
  }
} // namespace IKan
