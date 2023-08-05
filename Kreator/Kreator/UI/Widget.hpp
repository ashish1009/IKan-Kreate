//
//  Widget.hpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

namespace Kreator_UI
{  
  // This class manage the ImGui predefined widget like Search, Setting etc
  class Widgets
  {
  public:
    /// This function initialise the Widget Icons
    static void Initialize();
    /// This function remove widget icons
    static void Shutdown();
    
    /// This function Render the Search box Widget
    /// - Parameters:
    ///   - searchString: Search string update
    ///   - BuffSize: Size of text buffer
    ///   - hint: Hint to be shown in box
    ///   - Rounding of box
    /// - Note: Press Escape to cancel the search
    static bool SearchWidget(char* searchString, uint32_t BuffSize = 128, const char* hint = "Search...",
                             bool* grabFocus = nullptr, float roundingVal = 10);
    static bool AssetSearchPopup(const char* ID, AssetType assetType, AssetHandle& selected, bool allowMemoryOnlyAssets,
                                 bool* cleared = nullptr, const char* hint = "Search Assets",
                                 const ImVec2& size = ImVec2{ 250.0f, 350.0f });

  private:
    inline static Ref<Image> s_searchIcon;
    inline static Ref<Image> s_gearIcon;
    inline static char* s_assetSearchString;
  };
  
  bool IsMatchingSearch(const std::string& item, std::string_view searchQuery, bool caseSensitive = false,
                        bool stripWhiteSpaces = false, bool stripUnderscores = false);

} // namespace Kreator_UI
