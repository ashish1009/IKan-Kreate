//
//  Font.hpp
//  IKan
//
//  Created by Ashish . on 09/02/24.
//

#pragma once

namespace IKan
{
  class CharTexture;
  
  /// This class stores the font data
  class Font
  {
  public:
    /// This constructor creates a font instance
    /// - Parameter filePath: font file path
    Font(const std::filesystem::path& filePath);
    /// This function destroy the font
    ~Font();
    
    /// This function returns the font file path
    const std::filesystem::path& GetFilePath() const;
    
    /// This function initialize tha static data for fonts
    static void Initialize();
    /// This function shutdown tha static data for fonts
    static void Shutdown();
    
    /// This function returns the default font
    static Ref<Font> GetDefaultFont();
    
    /// This function creates a font instance
    /// - Parameter filePath: font file path
    static Ref<Font> Create(const std::filesystem::path& filePath);
        
  private:
    std::filesystem::path m_filePath {};
    std::map<char, Ref<CharTexture>> m_charTextureMap;
    
    inline static Ref<Font> s_defaultFont;
    friend class TextRenderer;
  };
} // namespace IKan
