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
  };

} // namespace IKan
