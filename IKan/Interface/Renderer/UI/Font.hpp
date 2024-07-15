//
//  Font.hpp
//  IKan
//
//  Created by Ashish . on 15/07/24.
//

#pragma once

namespace IKan
{
  class CharTexture;

  /// This class stores the font loaded as ttf file to be used to render the text
  class Font
  {
  public:
    /// This constructor creates the font instance with filepath
    /// - Parameter filePath: font filepath
    Font(const std::filesystem::path& filePath);
    /// This destructor destroys the font instance with filepath
    ~Font();

    // Getters -------------------------------------------------------------------------------------------------------
    /// This function returns the font file path
    const std::filesystem::path& GetFilePath() const;
    /// This function returns the char texture from map
    /// - Parameter charValue: char value
    Ref<CharTexture> GetCharTexture(char charValue);

    // Static APIs ---------------------------------------------------------------------------------------------------
    /// This function initializes tha static data for fonts
    static void Initialize();
    /// This function shuts down tha static data for fonts
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
  };
} // namespace IKan
