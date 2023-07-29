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
    
    /// This function initialise tha static data for fonts
    static void Initialise();
    /// This function shutdown tha static data for fonts
    static void Shutdown();
    /// This function creates a font instance
    /// - Parameter filePath: font file path
    static Ref<Font> Create(const std::string& filePath);
    
  private:
    inline static Ref<Font> s_defaultFont;
  };
} // namespace IKan

