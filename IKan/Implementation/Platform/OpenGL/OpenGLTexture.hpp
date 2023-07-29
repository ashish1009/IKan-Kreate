//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include <glad/glad.h>
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// This is the Open GL Implementation class for Creating texture
  class OpenGLImage : public Image
  {
  public:
    /// This constructor creates the texture using image file path
    /// - Parameters:
    ///   - filePath: Texture image file path (absolute)
    ///   - linear: min linear filter
    OpenGLImage(const std::string& filePath, bool linear);
    /// Default destructor that delete the texture
    virtual ~OpenGLImage();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the texture
    void Unbind() const override;
    
    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override;
    /// This function returns file path of texture
    const std::string& GetfilePath() const override;
    /// This function returns name of texture
    const std::string& GetName() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLImage);
    
  private:
    RendererID m_rendererID = 0;
    int32_t m_width = 1600;
    int32_t m_height = 900;
    int32_t m_channel = 0;
    uint32_t m_size = 0;
    std::string m_filePath = "", m_name = "";
  };

  /// This is the Open GL Implementation class for Creating texture
  class OpenGLTexture : public Texture2D
  {
  public:
    /// Default Open GL Image Constructor
    /// - Parameter spec: Open GL Image Specification
    OpenGLTexture(const TextureSpecification& spec);
    
    /// Default destructor that delete the texture
    virtual ~OpenGLTexture();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the texture
    void Unbind() const override;
    
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - id : Color Attaachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t id = 0) const override;

    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override;

  private:
    RendererID m_rendererID = 0;
    TextureSpecification m_specification;
    int32_t m_channel = 0;
  };
  
  class OpenGLCharTexture : public CharTexture
  {
  public:
    /// This constructor creates the Open GL Texture for rendering char
    /// - Parameters:
    ///   - face: face of char
    ///   - size: size of char
    ///   - bearing: bearing
    ///   - advance: advance
    ///   - charVal: character value
    OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing,
                      uint32_t advance, [[maybe_unused]] char charVal);
    /// This is the default Open GL Char Texture destructor
    virtual ~OpenGLCharTexture();
    
    /// This function binds the texture
    void Bind(uint32_t slot) const override;
    /// This function unbinds the texture
    void Unbind() const  override;
    
    /// This function returns the Renderer ID
    RendererID GetRendererID() const override;
    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns the size
    glm::ivec2 GetSize() const override;
    /// This function returns the bearing
    glm::ivec2 GetBearing() const override;
    /// This function returns the advance
    uint32_t GetAdvance() const override;

    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLCharTexture);
    
  private:
    RendererID m_rendererID;
    glm::ivec2 m_size;
    glm::ivec2 m_bearing;
    uint32_t m_advance = 0;
    uint32_t m_dataSize = 0;
    uint32_t m_width = 0, m_height = 0;
  };
  
  namespace TextureUtils
  {
    /// This function returns the Format name from Enum
    /// - Parameter format: enum taken as uint
    std::string IKanFormatName(TextureFormat format);
  } // namespace TextureUtils

} // namespace IKan
