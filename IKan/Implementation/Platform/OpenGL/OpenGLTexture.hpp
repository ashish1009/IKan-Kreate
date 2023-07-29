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
  class OpenGLTexture : public Texture
  {
  public:
    /// Default Open GL Image Constructor
    /// - Parameter spec: Open GL Image Specification
    OpenGLTexture(const Specification& spec);
    
    /// Default destructor that delete the texture
    virtual ~OpenGLTexture();
    
  private:
    RendererID m_rendererID = 0;
    Specification m_specification;
    int32_t m_channel = 0;
  };
  
  namespace TextureUtils
  {
    /// This function returns the Format name from Enum
    /// - Parameter format: enum taken as uint
    std::string IKanFormatName(TextureFormat format);
  } // namespace TextureUtils

} // namespace IKan
