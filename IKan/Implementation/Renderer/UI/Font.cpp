//
//  Font.cpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer/Graphics/Texture.hpp"

namespace IKan
{
  Ref<Font> Font::Create(const std::filesystem::path& filePath)
  {
    return CreateRef<Font>(filePath);
  }
  
  void Font::Initialize()
  {
    IK_PROFILE();
    
    // Default Font
    s_defaultFont = FontLibrary::Get(CoreAsset("Fonts/HfMonorita/Regular.ttf"));
  }
  
  void Font::Shutdown()
  {
    IK_PROFILE();
    s_defaultFont.reset();
  }
  
  Ref<Font> Font::GetDefaultFont()
  {
    return s_defaultFont;
  }

  Font::Font(const std::filesystem::path& fontFilePath)
  {
    IK_PROFILE();
    FT_Library ft = nullptr;
    [[maybe_unused]] bool initialized = FT_Init_FreeType(&ft);
    IK_ASSERT(!initialized, "Not able to load Freetype");
    
    // find path to font
    IK_ASSERT(std::filesystem::exists(fontFilePath), "Invalid Font path");
    
    // Load font as face
    FT_Face face = nullptr;
    [[maybe_unused]] bool faceCreated = FT_New_Face(ft, fontFilePath.c_str(), 0, &face);
    IK_ASSERT(!faceCreated, "Unavle to load tha font to freetype");
    
    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    // Load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++)
    {
      // Load character glyph
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        IK_LOG_CRITICAL(LogModule::Font, "ERROR::FREETYTPE: Failed to load Glyph");
        continue;
      }
      
      // Character Specificaion
      CharTextureSpecification spec;
      spec.face = face;
      spec.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
      spec.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
      spec.advance = static_cast<uint32_t>(face->glyph->advance.x);
      spec.charVal = static_cast<char>(c);
      
      // Now store character for later use
      Ref<CharTexture> charTexture = std::dynamic_pointer_cast<CharTexture>(TextureFactory::Create(spec));
      m_charTextureMap.insert(std::pair<char, Ref<CharTexture>>(c, charTexture));
    }
    
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    
    m_filePath = fontFilePath;
    
    IK_LOG_INFO(LogModule::Font, "Loaded the Font for Text renderer '{0}'", Utils::FileSystem::IKanAbsolute(fontFilePath.c_str()).string());
  }
  
  Font::~Font()
  {
    IK_PROFILE();
    m_charTextureMap.clear();
  }
  
  const std::filesystem::path& Font::GetFilePath() const
  {
    return m_filePath;
  }
  
  Ref<CharTexture> Font::GetCharTexture(char charValue)
  {
    IK_ASSERT(m_charTextureMap.find(charValue) != m_charTextureMap.end());
    return m_charTextureMap.at(charValue);
  }
  
  Ref<Font> FontLibrary::Get(const std::filesystem::path& fontFilePath)
  {
    IK_PROFILE();
    if (s_fonts.find(fontFilePath) == s_fonts.end() or !s_fonts.at(fontFilePath))
    {
      s_fonts[fontFilePath] = Font::Create(fontFilePath);
      IK_LOG_TRACE(LogModule::Font, "Font '{0}' will be added to Font Library. (Total Shaders {1})", fontFilePath.filename().string(), s_fonts.size());
    }
    else
    {
      IK_LOG_TRACE(LogModule::Font, "Returning Pre loaded Font '{0}' from Font Library", fontFilePath.filename().string());
    }
    return s_fonts.at(fontFilePath);
  }
  void FontLibrary::Clear()
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Font, "Removing all fonts from fonts Library");
    s_fonts.clear();
  }
} // namespace IKan
