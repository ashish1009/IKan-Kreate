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
} // namespace IKan
