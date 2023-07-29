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
  class Texture2D;
  
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
    
    /// This function binds the current FrameBuffer to the renderer
    virtual void Bind() const = 0;
    /// This function unbinds the current FrameBuffer from the renderer
    virtual void Unbind() const = 0;
    /// This function resizes the Framebuffer
    /// - Parameters:
    ///   - width: New width of FB
    ///   - height: New height of FB
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    
    /// This funciton updates the specification color
    /// - Parameter color: color
    virtual void UpdateSpecificationColor(const glm::vec4& color) = 0;
    
    /// This function returns the Frame buffer specification
    virtual const Specification& GetSpecification() const = 0;
    /// This function returns the Renderer ID
    virtual RendererID GetRendererId() const = 0;
    /// This function returns the Depth Attachment ID
    virtual Ref<Texture2D> GetDepthAttachment() const = 0;
    /// This function returns the Color Attachment iDs
    virtual const std::vector<Ref<Texture2D>>& GetColorAttachments() const = 0;
    /// This function returns the pixel id (RED_ID) index stored in fremebuffer
    virtual uint32_t GetPixelIdIndex() const = 0;

    /// This static function creates the Framebuffer instance based on the current Supported API
    /// - Parameter spec: Frame buffer specification
    [[nodiscard]] static Ref<FrameBuffer> Create(const Specification& spec);
  };
} // namespace IKan
