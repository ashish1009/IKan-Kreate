//
//  Font.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
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
    Font(const std::string& filePath);
    /// This function destroy the font
    ~Font();
    
    /// This function returns the font file path
    const std::string& GetFilePath() const;

    /// This function initialise tha static data for fonts
    static void Initialise();
    /// This function shutdown tha static data for fonts
    static void Shutdown();

    /// This function returns the default font
    static Ref<Font> GetDefaultFont();

    /// This function creates a font instance
    /// - Parameter filePath: font file path
    static Ref<Font> Create(const std::string& filePath);
    
  private:
    std::string m_filePath;
    std::map<char, Ref<CharTexture>> m_charTextureMap;
    inline static Ref<Font> s_defaultFont;
  };
} // namespace IKan

