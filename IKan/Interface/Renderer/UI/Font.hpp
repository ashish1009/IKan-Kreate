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
  
  /// This class stores the loaded fonts in a library (map)
  class FontLibrary
  {
  public:
    /// This function returns the pre loaded font from library
    /// - Parameter fontFilePath: font file path
    /// - Note: Creates new if not present in map
    static Ref<Font> Get(const std::filesystem::path& fontFilePath);
    /// This function clear the fonts loaded in library
    static void Clear();
    
    DELETE_ALL_CONSTRUCTORS(FontLibrary);
    
  private:
    inline static std::unordered_map<std::filesystem::path, Ref<Font>> s_fonts;
  };} // namespace IKan
