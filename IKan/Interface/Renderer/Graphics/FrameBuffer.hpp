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
    
    /// This structure stores the Framebuffer specification
    struct Specification
    {
      uint32_t width = 2100, height = 900;
      glm::vec4 color = { 0.1f, 0.1f, 0.1f, 1.0f };
      Attachments attachments;
    };

    /// Default virtual destructor
    virtual ~FrameBuffer() = default;
    
    /// This static function creates the Framebuffer instance based on the current Supported API
    /// - Parameter spec: Frame buffer specification
    [[nodiscard]] static Ref<FrameBuffer> Create(const Specification& spec);
  };
} // namespace IKan
