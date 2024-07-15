//
//  RendererBuffer.hpp
//  IKan
//
//  Created by Ashish . on 12/07/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"

namespace IKan
{
  // Vertex Buffer ---------------------------------------------------------------------------------------------------
  /// This class is the interface for creating renderer vertex buffer
  class VertexBuffer
  {
  public:
    /// This is the default virtual destuctor for vertex buffer
    virtual ~VertexBuffer() = default;

    /// This function updates the date in buffer dynamically
    /// - Parameters:
    ///   - data: Data pointer to be stored in GPU
    ///   - size: size of data
    ///   - offset: offset of data
    virtual void SetData(void* data, uint32_t size, uint32_t offset = 0) = 0;
 
    /// This function binds the Vertex Buffer before rendering
    virtual void Bind() const = 0;
    /// This function unbinds the Vertex Buffer after rendering
    virtual void Unbind() const = 0;
    
    /// This function returns the size of Vertex Buffer in GPU
    virtual uint32_t GetSize() const = 0;
    /// This function returns the renderer ID of Vertex Buffer
    virtual RendererID GetRendererID() const = 0;
  };
  
  // Index Buffer ---------------------------------------------------------------------------------------------------
  // Fixed size of single indices
  static constexpr uint32_t SizeOfSingleIndices = sizeof(uint32_t);
  /// This class is the interface for creating renderer index buffer
  class IndexBuffer
  {
  public:
    /// This is the default virtual destuctor for index buffer
    virtual ~IndexBuffer() = default;
    
    /// This function binds the Vertex Buffer before rendering
    virtual void Bind() const = 0;
    /// This function unbinds the Vertex Buffer after rendering
    virtual void Unbind() const = 0;
    
    /// This function returns the Number of Indices used by this Index Buffer
    virtual uint32_t GetCount() const = 0;
    /// This function returns the size of Index Buffer in GPU
    virtual uint32_t GetSize() const = 0;
    /// This function returns the renderer ID of Index Buffer
    virtual RendererID GetRendererID() const = 0;
  };

} // namespace IKan
