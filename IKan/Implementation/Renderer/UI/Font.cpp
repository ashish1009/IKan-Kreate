//
//  Font.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Font.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  Ref<Font> Font::Create(const std::string& filePath)
  {
    return CreateRef<Font>(filePath);
  }
  
  void Font::Initialize()
  {
    // Default Font
    std::string defaultFontFilePath = CoreAssetPath("Fonts/HfMonorita/Regular.ttf");
    s_defaultFont = Font::Create(defaultFontFilePath);
  }
  
  void Font::Shutdown()
  {
    s_defaultFont.reset();
  }
  
  Ref<Font> Font::GetDefaultFont()
  {
    return s_defaultFont;
  }

  Font::Font(const std::string& fontFilePath)
  {
    FT_Library ft;
    IK_ASSERT(!FT_Init_FreeType(&ft), "Not able to load Freetype");
    
    // find path to font
    IK_ASSERT(Utils::FileSystem::Exists(fontFilePath), "Invalid Font path");
    
    // load font as face
    FT_Face face;
    IK_ASSERT(!FT_New_Face(ft, fontFilePath.c_str(), 0, &face), "Unavle to load tha font to freetype");
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        IK_LOG_CRITICAL(LogModule::Font, "ERROR::FREETYTPE: Failed to load Glyph");
        continue;
      }
      // now store character for later use
      Ref<CharTexture> charTexture = CharTexture::Create(face,
                                                         glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                                         glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                                         static_cast<uint32_t>(face->glyph->advance.x),
                                                         static_cast<char>(c));
      m_charTextureMap.insert(std::pair<char, Ref<CharTexture>>(c, charTexture));
    }
    
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    m_filePath = fontFilePath;
    
    IK_LOG_TRACE(LogModule::Font, "Loaded the Font for Text renderer '{0}'", fontFilePath.c_str());
  }
  
  Font::~Font()
  {
    m_charTextureMap.clear();
  }
  
  const std::string& Font::GetFilePath() const
  {
    return m_filePath;
  }

} // namespace IKan
