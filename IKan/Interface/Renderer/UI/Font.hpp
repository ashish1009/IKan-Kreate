//
//  Font.hpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#pragma once

#include "Asset/Asset.hpp"

namespace IKan
{
  class CharTexture;
  
  /// This class stores the font data
  class Font : public Asset
  {
  public:
    /// This constructor creates a font instance
    /// - Parameter filePath: font file path
    Font(const std::string& filePath);
    /// This function destroy the font
    ~Font();
    
    /// This function returns the font file path
    const std::string& GetFilePath() const;

    /// This function initialize tha static data for fonts
    static void Initialize();
    /// This function shutdown tha static data for fonts
    static void Shutdown();

    /// This function returns the default font
    static Ref<Font> GetDefaultFont();

    /// This function creates a font instance
    /// - Parameter filePath: font file path
    static Ref<Font> Create(const std::string& filePath);
    
    ASSET_TYPE(Font);
    
  private:
    std::string m_filePath;
    std::map<char, Ref<CharTexture>> m_charTextureMap;
    inline static Ref<Font> s_defaultFont;
    friend class TextRenderer;
  };
} // namespace IKan

