//
//  Specifications.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Renderer/Graphics/BufferLayout.hpp"

// Freetype forward declaration
typedef struct FT_FaceRec_*  FT_Face;

namespace IKan
{
  // Forward declaration
  class Shader;

  // Texture Specifications ------------------------------------------------------------------------------------------
  /// This enum stores the attachment type of texture
  enum class TextureAttachment : uint8_t
  {
    None = 0,
    Color, Depth
  };
  
  /// This enum stores the texture type supported by IKan
  enum class TextureType : uint8_t
  {
    Texture2D,
    TextureCubemap
  };
  
  /// This enum stores the texture format supported
  enum class TextureFormat : uint8_t
  {
    None = 0,
    RGBA16F, RGBA8, RGBA, RGB8, RGB,
    RED,
    R32I, RED_INTEGER,
    DEPTH_COMPONENT
  };
  
  /// This enum stores the erap type of texture
  enum class TextureWrap : uint8_t
  {
    Repeat, RepeatMirror, ClampEdge, ClampBorder
  };
  
  /// This enum stores the texture filter
  enum class TextureFilter : uint8_t
  {
    Linear, Nearest, LinearMipmapLinear, NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest
  };
  
  /// This structure stores the Texture specification data
  struct Texture2DSpecification
  {
#ifdef __APPLE__
    /// Maximum Texture slot supported by Senderer Shader Current Open GL Version 4.1 supports only 16 Texture slot in Shader
    static constexpr uint32_t MaxTextureSlotsInShader = 16;
#else
    static constexpr uint32_t MaxTextureSlotsInShader = 0;
#endif

    std::string title {"Empty Texture"};
    
    uint32_t width {1600};
    uint32_t height {900};

    TextureType type {TextureType::Texture2D};
    TextureFormat internalFormat {TextureFormat::RGBA8};
    TextureFormat dataFormat {TextureFormat::RGBA};
    
    TextureWrap Xwrap {TextureWrap::Repeat};
    TextureWrap Ywrap {TextureWrap::Repeat};
    TextureWrap Zwrap {TextureWrap::Repeat};
    
    TextureFilter minFilter {TextureFilter::Linear};
    TextureFilter magFilter {TextureFilter::Linear};
    
    void* data {nullptr};
    uint32_t size {0};
    
    bool enableDebugLogs {true};
  };
  
  /// This structure stores the Texture specificaion for character rendering
  struct CharTextureSpecification
  {
    FT_Face face;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
    char charVal;
  };

  // Pipeline Specification ------------------------------------------------------------------------------------------
  struct PipelineSpecification
  {
    std::string debugName {};
    BufferLayout vertexLayout;
    Ref<Shader> shader;
  };
} // namespace IKan
