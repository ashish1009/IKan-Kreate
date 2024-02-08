//
//  OpenGLTexture.hpp
//  IKan
//
//  Created by Ashish . on 08/02/24.
//

#pragma once

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  /// Interface class for storing Texture
  class OpenGLTexture : public Texture
  {
  public:
    /// Default Open GL Image Constructor
    /// - Parameter spec: Open GL Image Specification
    OpenGLTexture(const Texture2DSpecification& spec);
    /// This is the default frame buffer destructor that delete the buffer
    ~OpenGLTexture();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the current Texture from the renderer
    void Unbind() const override;
    
    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override;
    
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - colorID : Color Attaachment ID to Framebuffer
    ///   - depthID : Depth Attachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLTexture);
    
  private:
    RendererID m_rendererID {0};
    Texture2DSpecification m_specification;
    int32_t m_channel {0};
  };

  /// This is the Open GL Implementation class for Creating texture
  class OpenGLImage : public Image
  {
  public:
    /// Default Open GL Image Constructor
    /// - Parameter spec: Open GL Image Specification
    OpenGLImage(const ImageSpecificaion& spec);
    /// Default destructor that delete the texture
    virtual ~OpenGLImage();
    
    /// This function binds the texture at slot
    /// - Parameter slot: shader slot where this texture to be binded
    void Bind(uint32_t slot = 0) const override;
    /// This function unbinds the texture
    void Unbind() const override;
    
    /// This function attach the current Image to Framebuffer
    /// - Parameters:
    ///   - colorID : Color Attaachment ID to Framebuffer
    ///   - depthID : Depth Attachment ID to Framebuffer
    ///   - attachmentType: attachment type of texture
    void AttachToFramebuffer(TextureAttachment attachmentType, uint32_t colorID = 0, uint32_t depthID = 0, uint32_t level = 0) const override {IK_ASSERT(false);}
    
    /// This function returns width of texture
    uint32_t GetWidth() const override;
    /// This function returns height of texture
    uint32_t GetHeight() const override;
    /// This function returns renderer ID of texture
    RendererID GetRendererID() const override;
    /// This function returns file path of texture
    const std::filesystem::path& GetfilePath() const override;
    /// This function returns name of texture
    const std::string& GetName() const override;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(OpenGLImage);
    
  private:
    RendererID m_rendererID {0};
    ImageSpecificaion m_specification;
    int32_t m_width {1600};
    int32_t m_height {900};
    int32_t m_channel {0};
    uint32_t m_size {0};
    std::string m_name {};
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
    OpenGLCharTexture(const FT_Face& face, const glm::ivec2& size, const glm::ivec2& bearing, uint32_t advance, [[maybe_unused]] char charVal);
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
    RendererID m_rendererID {};
    glm::ivec2 m_size;
    glm::ivec2 m_bearing;
    uint32_t m_advance {0};
    uint32_t m_dataSize {0};
    uint32_t m_width {0}, m_height {0};
  };
} // namespace IKan
