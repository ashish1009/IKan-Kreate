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
    std::string debugName {};
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

  struct Texture2DSpecification
  {
    int32_t width {1600};
    int32_t height {900};
    
    TextureFormat internalFormat {TextureFormat::RGBA8};
    TextureFormat dataFormat {TextureFormat::RGBA};
    TextureType type {TextureType::Texture2D};
    TextureWrap wrap {TextureWrap::Repeat};
    TextureFilter filter {TextureFilter::Linear};
    
    void* data {nullptr};
    uint32_t size {0};
  };
  
  struct ImageSpecificaion
  {
    bool invertVertically {true};
    std::filesystem::path filePath {};
    TextureWrap wrap {TextureWrap::Repeat};
    TextureFilter filter {TextureFilter::Linear};
  };
  
  // Framebuffer specificaion ------------------------------------------------------------------------------
  struct FrameBufferAttachments
  {
    /// Texture format type
    enum class TextureFormat
    {
      None = 0,
      RGBA8, RGBA16F,
      R32I, RED,
      Depth24Stencil,
      DepthCubeMap
    };
    
    FrameBufferAttachments() = default;
    FrameBufferAttachments(std::initializer_list<TextureFormat> attachments)
    : textureFormats(attachments) {}
    std::vector<TextureFormat> textureFormats;
  };
  
  /// This structure stores the Framebuffer specification
  struct FrameBufferSpecification
  {
    std::string debugName {};
    uint32_t width {2100}, height{900};
    glm::vec4 color { 0.1f, 0.1f, 0.1f, 1.0f };
    FrameBufferAttachments attachments;
  };
} // namespace IKan
