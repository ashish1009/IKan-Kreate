//
//  OpenGLRendererBuffer.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Graphics/RendererBuffer.hpp"

namespace IKan
{
  // Vertex Buffer ---------------------------------------------------------------------------------------------------
  /// This class implements the APIs for creating and storing vertex buffer data.
  class OpenGLVertexBuffer : public VertexBuffer
  {
  public:
    /// This Constructor creates the vertex buffer. Buffer data is passed and stored as static in GPU.
    /// - Parameters:
    ///   - data: Data of vertex Buffer
    ///   - size: Size of vertex Buffer
    OpenGLVertexBuffer(void* data, uint32_t size);
    /// This Constructor creates the vertex buffer. Buffer size is passed to reserve the memory in GPU. Data to passed.
    /// later dynamically.
    /// - Parameter size: Size of vertex buffer.
    OpenGLVertexBuffer(uint32_t size);
    
    /// This destructor destroyes the Open GL Vertex Buffer.
    ~OpenGLVertexBuffer();
  };
} // namespace IKan
