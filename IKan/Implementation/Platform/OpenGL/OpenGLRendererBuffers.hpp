//
//  OpenGLRendererBuffers.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Graphics/RendererBuffers.hpp"

namespace IKan
{
  /// This is the implementation class for creating Renmderer Vertex Buffer
  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    /// This Constructor creates the vertex buffer. Buffer data is passed and stored as static in GPU
    /// - Parameters:
    ///   - data: Data of vertex Buffer
    ///   - size: Size of vertex Buffer
    OpenGLVertexBuffer(void* data, uint32_t size);
    /// This Constructor creates the vertex buffer. Buffer size is passed to reserve the memory in GPU. Data to passed
    /// later dynamically
    /// - Parameter size: Size of vertex buffer
    OpenGLVertexBuffer(uint32_t size);
    
    /// This is the default destructor for Open GL Vertex Buffer
    ~OpenGLVertexBuffer();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLVertexBuffer);
    
  private:
    RendererID m_rendererID;
    uint32_t m_size = 0;
  };

  /// This is the implementation class to create Index Buffer
  class OpenGLIndexBuffer : public IndexBuffer
  {
  public:
    /// This Constructor create the Index Buffer with Indices data and its size
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - size: size of data in index buffer
    OpenGLIndexBuffer(void *data, uint32_t size);
    /// This is the default destructor for Open GL Index buffer
    ~OpenGLIndexBuffer();
    
    /// This function binds the Vertex Buffer before rendering
    void Bind() const override;
    /// This function unbinds the Vertex Buffer after rendering
    void Unbind() const override;
    
    /// This function returns the Number of Indices used by this Index Buffer
    uint32_t GetCount() const override;
    /// This function returns the size of Index Buffer in GPU
    uint32_t GetSize() const override;
    /// This function return the renderer ID of Index Buffer
    RendererID GetRendererID() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLIndexBuffer);
    
  private:
    RendererID m_rendererID;
    uint32_t m_size = 0;
  };
} // namespace IKan
