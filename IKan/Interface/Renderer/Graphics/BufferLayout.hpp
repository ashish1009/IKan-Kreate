//
//  BufferLayout.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

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
} // namespace IKan
