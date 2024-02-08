//
//  OpenGLRendererBuffer.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/RendererBuffer.hpp"

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
    
    /// This function update the date in buffer dynamically
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    ///   - offset: data offset
    void SetData(void* data, uint32_t size, uint32_t offset = 0) override;
    /// This function binds the Vertex Buffer before rendering
    void Bind() const override;
    /// This function unbinds the Vertex Buffer after rendering
    void Unbind() const override;
    
    /// This function returns the renderer ID of Vertex Buffer
    RendererID GetRendererID() const override;
    /// This function returns the size of Vertex Buffer in GPU
    uint32_t GetSize() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLVertexBuffer);
    
  private:
    RendererID m_rendererID;
    uint32_t m_size {0};
  };

} // namespace IKan
