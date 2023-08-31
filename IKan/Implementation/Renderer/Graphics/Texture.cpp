//
//  Texture.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Texture.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace IKan
{
  
  Ref<Texture> Texture::Create(const TextureSpecification& spec)
  {
    return Texture2D::Create(spec);
  }
  
  Ref<Texture> Texture::Create(const std::string& filePath, bool linear)
  {
    return Image::Create(filePath, linear);
  }
  
  Ref<Texture> Texture::CreateWhiteTexture()
  {
    // White data
    static uint32_t whiteTextureData = 0xffffffff;
    
    // Texture specificaion
    TextureSpecification textureSpec;
    textureSpec.type = TextureType::Texture2D;
    textureSpec.internalFormat = TextureFormat::RGBA8;
    textureSpec.dataFormat = TextureFormat::RGBA;
    textureSpec.wrap = TextureWrap::Clamp;
    textureSpec.filter = TextureFilter::Linear;
    textureSpec.width = 1;
    textureSpec.height = 1;
    textureSpec.data = &whiteTextureData;
    textureSpec.size = sizeof(uint32_t);

    return Texture2D::Create(textureSpec);
  }
  
  Ref<Texture2D> Texture2D::Create(const TextureSpecification& spec)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLTexture>(spec);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API");
    }
  }

  Ref<Image> Image::Create(const std::string& filePath, bool linear)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl:
      {
        if (s_textureLibrary.find(filePath) == s_textureLibrary.end() or !s_textureLibrary[filePath][linear])
        {
          s_textureLibrary[filePath][linear] = CreateRef<OpenGLImage>(filePath, linear);
          IK_LOG_DEBUG(LogModule::Texture, "Adding Texture '{0}' to Texture Library",
                       Utils::String::GetFileNameFromPath(filePath));
          IK_LOG_DEBUG(LogModule::Texture, "Number of Textures loaded {0}",
                       s_textureLibrary.size());
        }
        else
        {
          IK_LOG_DEBUG(LogModule::Texture, "Returning Pre loaded Texture '{0}' from Texture Library",
                       Utils::String::GetFileNameFromPath(filePath));
        }
        return s_textureLibrary.at(filePath)[linear];
      }
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }
  
  void Image::ResetLibrary()
  {
    IK_PROFILE();
    for (auto it = s_textureLibrary.begin(); it != s_textureLibrary.end(); it++)
    {
      IK_LOG_DEBUG(LogModule::Texture, "Removing Texture '{0}' from Texture Library",
                   Utils::String::GetFileNameFromPath(it->first));
      for (int i = 0; i < 2; i++)
      {
        it->second[i].reset();
      }
    }
  }

  Sprite::Sprite(const Ref<Image>& spriteImage, const glm::vec2& min, const glm::vec2& max, const glm::vec2& coords,
                 const glm::vec2& spriteSize, const glm::vec2& cellSize)
  : m_spriteImage(spriteImage), m_spriteSize(spriteSize), m_cellSize(cellSize), m_coords(coords)
  {
    m_textureCoord[0] = {min.x, min.y};
    m_textureCoord[1] = {max.x, min.y};
    m_textureCoord[2] = {max.x, max.y};
    m_textureCoord[3] = {min.x, max.y};
  }
  
  Ref<Sprite> Sprite::Create(const Ref<Image>& spriteImage, const glm::vec2& coords, const glm::vec2& spriteSize,
                             const glm::vec2& cellSize) {
    glm::vec2 min =
    {
      (coords.x * cellSize.x) / spriteImage->GetWidth(),
      (coords.y * cellSize.y) / spriteImage->GetHeight()
    };
    glm::vec2 max =
    {
      ((coords.x + spriteSize.x) * cellSize.x) / spriteImage->GetWidth(),
      ((coords.y + spriteSize.y) * cellSize.y) / spriteImage->GetHeight()
    };
    
    IK_LOG_TRACE(LogModule::SubTexture, "Creating Sub Texture with following Data ");
    IK_LOG_TRACE(LogModule::SubTexture, "  Sprite Image  {0}", spriteImage->GetName());
    IK_LOG_TRACE(LogModule::SubTexture, "  Coordinates   {0} : {1}", coords.x, coords.y);
    IK_LOG_TRACE(LogModule::SubTexture, "  Sprite Size   {0} : {1}", spriteSize.x, spriteSize.y);
    IK_LOG_TRACE(LogModule::SubTexture, "  Cell Size     {0} : {1}", cellSize.x, cellSize.y);
    IK_LOG_TRACE(LogModule::SubTexture, "  Min Bound     {0} : {1}", min.x, min.y);
    IK_LOG_TRACE(LogModule::SubTexture, "  Max Bound     {0} : {1}", max.x, max.y);
    
    return CreateRef<Sprite>(spriteImage, min, max, coords, spriteSize, cellSize);
  }
  
  Ref<Image> Sprite::GetImage()
  {
    return m_spriteImage;
  }
  const glm::vec2* Sprite::GetTexCoord() const
  {
    return m_textureCoord;
  }
  glm::vec2& Sprite::GetSpriteSize()
  {
    return m_spriteSize;
  }
  glm::vec2& Sprite::GetCellSize()
  {
    return m_cellSize;
  }
  glm::vec2& Sprite::GetCoords()
  {
    return m_coords;
  }

  Ref<CharTexture> CharTexture::Create(const FT_Face &face, const glm::ivec2 &size, const glm::ivec2 &bearing,
                                       uint32_t advance, [[maybe_unused]] char charVal)
  {
    switch (Renderer::GetApi())
    {
      case Renderer::Api::OpenGl: return CreateRef<OpenGLCharTexture>(face, size, bearing, advance, charVal);
      case Renderer::Api::None:
      default:
        IK_ASSERT(false, "Invalid Renderer API (None)"); break;
    }
  }

} // namespace IKan
