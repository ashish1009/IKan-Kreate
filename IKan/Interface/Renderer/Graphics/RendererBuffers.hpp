//
//  RendererBuffers.hpp
//  IKan
//
//  Created by Ashish . on 28/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
  /// This enum shoes the data type to be stored in shader
  enum class ShaderDataType : uint8_t
  {
    NoType = 0,
    Float, Float2, Float3, Float4,
    Mat3, Mat4,
    Int, Int2, Int3, Int4,
    Bool
  };
  
  // Buffer Element ---------------------------------------------------------------------------------------------------
  /// This structur stores all the property of a element in a buffer.
  struct BufferElement
  {
    std::string name = "";
    ShaderDataType type = ShaderDataType::NoType;
    uint32_t size = 0;
    uint32_t count = 0;
    size_t offset = 0;
    bool normalized = false;
    
    /// This Constructor creates the buffer element with arguments
    /// - Parameters:
    ///   - name: name of element
    ///   - type: type of element
    ///   - normalized: flag is element normalized
    BufferElement(const std::string& name, ShaderDataType type, bool normalized = false);
  };
  
  // Fixed size of single indices
  static constexpr uint32_t SizeOfSingleIndices = sizeof(uint32_t);
  
  /// This is the interface class to create Index Buffer
  class IndexBuffer
  {
  public:
    /// This is the virtual default Index Buffer Destructor
    ~IndexBuffer() = default;

    /// This function binds the Vertex Buffer before rendering
    virtual void Bind() const = 0;
    /// This function unbinds the Vertex Buffer after rendering
    virtual void Unbind() const = 0;
    
    /// This function returns the Number of Indices used by this Index Buffer
    virtual uint32_t GetCount() const = 0;
    /// This function returns the size of Index Buffer in GPU
    virtual uint32_t GetSize() const = 0;
    /// This function return the renderer ID of Index Buffer
    virtual RendererID GetRendererID() const = 0;

    /// This function create the Index Buffer with Indices data and its size
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - size: size of data in index buffer
    [[nodiscard]] static Ref<IndexBuffer> CreateWithSize(void* data, uint32_t size);
    /// This function create the Index Buffer with Indices data and number of indices
    /// - Parameters:
    ///   - data: data to be filled in index buffer
    ///   - count: number of indices in index buffer
    /// - Note: Size of single indices is taken as size of uint32_t (4)
    [[nodiscard]] static Ref<IndexBuffer> CreateWithCount(void* data, uint32_t count);    
  };
} // namespace IKan
