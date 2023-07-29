//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Renderer/Renderer.hpp"

namespace IKan
{
  enum class TextureFormat
  {
    None = 0,
    RGBA8, RGBA, RGB8, RGB, RED,
    R32I, RED_INTEGER,
    DEPTH_COMPONENT
  };
  
  enum class TextureType
  {
    None = 0,
    Texture2D,
  };
  
  enum class TextureFilter
  {
    None = 0,
    Linear,
    Nearest,
  };
  
  enum class TextureWrap
  {
    None = 0,
    Clamp,
    Repeat
  };
  
  enum class TextureAttachment
  {
    None = 0,
    Color, Depth
  };

  class Image
  {
  public:
    /// This is default virtual destructor for Texture
    virtual ~Image() = default;
    
    /// This function binds the Current Texture to a slot of shader
    /// - Parameter slot: Slot of shader
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the Current Texture from shader slot
    virtual void Unbind() const = 0;

    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the File Path of Texture NOTE: Return "" for white texture
    virtual const std::string& GetfilePath() const = 0;
    /// This function returns name of texture
    virtual const std::string& GetName() const = 0;

    /// This static functions creates the Texture from image file
    /// - Parameters:
    ///   - filePath: path of texture file
    ///   - linear: min linear flag
    /// - Note: Use Renderer::GetTexture() to reuse already loaded texture (if following texture is already used
    ///         somewhere). This API will Load the texture again and new memory will be creted. So to avoid duplicate
    ///         memory for same texture use Renderer::GetTexture()
    [[nodiscard]] static Ref<Image> Create(const std::string& filePath, bool linear = true);
    
    /// This function deletes all the Texture present int the map
    static void ResetLibrary();
    
  private:
    // Array of 2 to keep both linear and nearest min and mag flags
    // 0 -> Linear Filter
    // 1 -> Nearest Filter
    inline static std::unordered_map<std::string /* File Path */, std::array<Ref<Image>, 2>> s_textureLibrary;
  };
  
  /// Wrepper class to load texture and render as sprite
  class Sprite
  {
  public:
    /// This Funtion create the subtexture instance with texture reference pointer and following params
    /// - Parameters:
    ///   - image: sprite_image
    ///   - min: min bound of sub texture
    ///   - max: mxn bound of sub texture
    ///   - coords: coordinate sof sub image from sprite image with reference to the cell Size.
    ///             e.g {1, 2} -> from top left corner of image use the block of size 'cell size'
    ///             from 16th pixel frol eft and 32nd pixled from top (using 16 assuming cell size is {16, 17})
    ///   - spriteSize: number of blocks to be taken at one time from sprite
    ///   - cellSize: size of block in pixel to be used for 1 block
    Sprite(const Ref<Image>& image,const glm::vec2& min,const glm::vec2& max, const glm::vec2& coords = {0.0f, 0.0f},
           const glm::vec2& spriteSize = {1.0f, 1.0f}, const glm::vec2& cellSize = {16.0f, 16.0f});
    
    /// Default destructor
    ~Sprite() = default;
    
    /// This function returns the texture reference of sprite
    Ref<Image> GetImage();
    /// This function returns Texture coordinates of Subtexture
    const glm::vec2* GetTexCoord() const;
    /// This function returns Sprite size of Subtexture
    glm::vec2& GetSpriteSize();
    /// This function returns Cell Size of Subtexture
    glm::vec2& GetCellSize();
    /// This function returnsCoordinates of Subtexture
    glm::vec2& GetCoords();
    
    /// This Funtion create the subtexture instance with texture reference pointer and following params
    /// - Parameters:
    ///   - spriteImage: sprite_image
    ///   - coords: coordinate sof sub image from sprite image with reference to the cell Size.
    ///             e.g {1, 2} -> from top left corner of image use the block of size 'cell size'
    ///             from 16th pixel frol eft and 32nd pixled from top (using 16 assuming cell size is {16, 17})
    ///   - spriteSize: number of blocks to be taken at one time from sprite
    ///   - cellSize: size of block in pixel to be used for 1 block
    [[nodiscard]] static Ref<Sprite> Create(const Ref<Image>& spriteImage, const glm::vec2& coords,
                                            const glm::vec2& spriteSize = {1.0f, 1.0f},
                                            const glm::vec2& cellSize = {16.0f, 16.0f});
    
  private:
    Ref<Image> m_spriteImage;
    glm::vec2 m_textureCoord[4];
    glm::vec2 m_spriteSize;
    glm::vec2 m_cellSize;
    glm::vec2 m_coords;
  };

  /// This is the interface class to create the Texture
  class Texture
  {
  public:
    /// This is the NON Image Texture specification
    struct Specification
    {
      int32_t width = 1600;
      int32_t height = 900;
      
      TextureFormat internalFormat = TextureFormat::RGBA8;
      TextureFormat dataFormat = TextureFormat::RGBA;
      TextureType type = TextureType::Texture2D;
      TextureWrap wrap = TextureWrap::Clamp;
      TextureFilter filter = TextureFilter::Linear;
      
      void* data = nullptr;
      uint32_t size = 0;
    };
    
    /// This is default virtual destructor for Texture
    virtual ~Texture() = default;

    /// This function binds the Current Texture to a slot of shader
    /// - Parameter slot: Slot of shader
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the Current Texture from shader slot
    virtual void Unbind() const = 0;
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - id : Color Attaachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    virtual void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t id = 0) const = 0;

    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;

    /// This API creates the Texture instance based on the Renderer API
    /// - Parameter spec: Texture Specificaion
    [[nodiscard]] static Ref<Texture> Create(const Specification& spec);
  };
} // namespace IKan
