//
//  FrameBuffer.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
  /// Interface class for storing Framebuffer
  class FrameBuffer
  {
  public:
    /// This structure stores the Attachments of Frame buffer. NOTE: Separate class to use initializer list in constructor
    /// of FrameBuffer::Specification
    struct Attachments
    {
      /// Texture format type
      enum class TextureFormat
      {
        None = 0,
        RGBA8,
        R32I,
        Depth24Stencil
      };
      
      Attachments() = default;
      Attachments(std::initializer_list<TextureFormat> attachments);
      std::vector<TextureFormat> textureFormats;
    };
  };
} // namespace IKan
