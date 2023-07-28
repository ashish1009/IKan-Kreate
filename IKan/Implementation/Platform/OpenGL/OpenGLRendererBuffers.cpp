//
//  OpenGLRendererBuffers.cpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#include <glad/glad.h>
#include "OpenGLRendererBuffers.hpp"
#include "Renderer/RendererStats.hpp"

namespace IKan
{
  OpenGLVertexBuffer::OpenGLVertexBuffer(void *data, uint32_t size)
  : m_size(size)
  {
    glGenBuffers(1, &m_rendererID);
    RendererStatistics::Get().vertexBufferSize += size;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
    
    IK_LOG_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer with Data ");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "-----------------------------------------");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", m_size);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().vertexBufferSize);
  }
  
  OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
  : m_size(size)
  {
    glGenBuffers(1, &m_rendererID);
    RendererStatistics::Get().vertexBufferSize += m_size;
    
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, GL_DYNAMIC_DRAW);
    
    IK_LOG_DEBUG(LogModule::VertexBuffer, "Creating Open GL Vertex Buffer without Data ");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "--------------------------------------------");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", m_size);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Used   {0} B", RendererStatistics::Get().vertexBufferSize);
  }
  
  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    RendererStatistics::Get().vertexBufferSize -= m_size;
    
    IK_LOG_DEBUG(LogModule::VertexBuffer, "Destroying Open GL Vertex Buffer ");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "---------------------------------");
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Renderer ID       {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Size              {0} B", m_size);
    IK_LOG_DEBUG(LogModule::VertexBuffer, "  Total Size Left   {0} B", RendererStatistics::Get().vertexBufferSize);
    
    glDeleteBuffers(1, &m_rendererID);
  }

  OpenGLIndexBuffer::OpenGLIndexBuffer(void *data, uint32_t size)
  : m_size(size)
  {
    glGenBuffers(1, &m_rendererID);
    RendererStatistics::Get().indexBufferSize += m_size;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, GL_STATIC_DRAW);
    
    IK_LOG_DEBUG(LogModule::IndexBuffer, "Creating Open GL Index Buffer ");
    IK_LOG_DEBUG(LogModule::IndexBuffer, "------------------------------");
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Renderer ID            {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Size of single Indices {0} B", SizeOfSingleIndices);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Size                   {0} B", m_size);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Total Size Used        {0} B", RendererStatistics::Get().indexBufferSize);
  }
  
  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    RendererStatistics::Get().indexBufferSize -= m_size;
    glDeleteBuffers(1, &m_rendererID);
    
    IK_LOG_DEBUG(LogModule::IndexBuffer, "Destroying Open GL Index Buffer ");
    IK_LOG_DEBUG(LogModule::IndexBuffer, "--------------------------------");
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Renderer ID            {0}", m_rendererID);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Size of single Indices {0} B", SizeOfSingleIndices);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Size                   {0} B", m_size);
    IK_LOG_DEBUG(LogModule::IndexBuffer, "  Total Size Left        {0} B", RendererStatistics::Get().indexBufferSize);
  }
  
  void OpenGLIndexBuffer::Bind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
  }
  
  void OpenGLIndexBuffer::Unbind() const
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
