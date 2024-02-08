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
} // namespace IKan
