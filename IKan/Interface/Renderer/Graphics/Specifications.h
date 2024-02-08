//
//  Specifications.h
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/BufferLayout.hpp"

namespace IKan
{
  struct PipelineSpecification
  {
    std::string debugName;
    BufferLayout vertexLayout;
  };
  
  // Texture Specificaions ---------------------------------------------
  enum class TextureAttachment
  {
    None = 0,
    Color, Depth
  };
  
  enum class TextureWrap
  {
    Repeat, RepeatMirror, ClampEdge, ClampBorder
  };
  
  enum class TextureFilter
  {
    Linear, LinearMipmapLinear, Nearest
  };
  
  enum class TextureType
  {
    Texture2D,
    TextureCubemap
  };
  
  enum class TextureFormat
  {
    None = 0,
    RGBA16F, RGBA8, RGBA, RGB8, RGB,
    RED,
    R32I, RED_INTEGER,
    DEPTH_COMPONENT
  };

} // namespace IKan
