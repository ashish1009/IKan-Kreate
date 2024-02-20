//
//  Texture.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Graphics/Factories.hpp"
#include "Renderer/Graphics/Specifications.h"
#include "Asset/Asset.hpp"

namespace IKan
{
  /// Interface class for storing Texture with specificaion data
  class Texture
  {
  public:
    /// Default virtual destructor
    virtual ~Texture() = default;
    
    /// This function binds the current Texture to the renderer
    virtual void Bind(uint32_t slot = 0) const = 0;
    /// This function unbinds the current Texture from the renderer
    virtual void Unbind() const = 0;
    
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - colorID : Color Attaachment ID to Framebuffer
    ///   - depthID : Depth Attachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    virtual void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const = 0;
    
    /// This function returns the Renderer ID of Texture
    virtual RendererID GetRendererID() const = 0;
    /// This function returns the Width of Texture
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the Height of Texture
    virtual uint32_t GetHeight() const = 0;
  };

  /// This is the interface class to create the Texture 2D for Framebuffers
  class Texture2D : public Texture
  {
  public:
    /// This is default virtual destructor for Texture
    virtual ~Texture2D() = default;
  };
  
  /// This is the interface class to create Image
  class Image : public Texture, public Asset
  {
  public:
    /// This is default virtual destructor for Texture
    virtual ~Image() = default;
    
    /// This function returns the File Path of Texture NOTE: Return "" for white texture
    virtual const std::filesystem::path& GetfilePath() const = 0;
    /// This function returns name of texture
    virtual const std::string& GetName() const = 0;
    
    ASSET_TYPE(Image);
  };
  
  class CharTexture
  {
  public:
    /// Default virtual destructor
    virtual ~CharTexture() = default;
    
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
    /// This function returns the Size of Freetpe face
    virtual glm::ivec2 GetSize() const = 0;
    /// This function returns the Bearing of Freetpe face
    virtual glm::ivec2 GetBearing() const = 0;
    /// This function returns the Advance of Freetpe face
    virtual uint32_t GetAdvance() const = 0;
  };
  
  namespace TextureUtils
  {
    /// This function returns the Format name from Enum
    /// - Parameter format: enum taken as uint
    const char* IKanFormatName(TextureFormat format);
  } // namespace TextureUtils

} // namespace IKan
