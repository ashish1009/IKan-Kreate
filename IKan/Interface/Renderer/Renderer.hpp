//
//  Renderer.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

namespace IKan
{
  /// Data type of Renderer ID
  using RendererID = uint32_t;
  
  enum class RendererType : uint8_t
  {
    Invalid, OpenGL
  };
} // namespace IKan
