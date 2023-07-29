//
//  Font.cpp
//  IKan
//
//  Created by Ashish . on 29/07/23.
//

#include "Font.hpp"

namespace IKan
{
  Ref<Font> Font::Create(const std::string& filePath)
  {
    return CreateRef<Font>(filePath);
  }
  
  void Font::Initialise()
  {
    // Default Font
    std::string defaultFontFilePath = CoreAssetPath("Fonts/HfMonorita/Regular.ttf");
    s_defaultFont = Font::Create(defaultFontFilePath);
  }
  
  void Font::Shutdown()
  {
    s_defaultFont.reset();
  }
  
  Font::Font(const std::string& filePath)
  {
    
  }
  Font::~Font()
  {
    
  }
} // namespace IKan
