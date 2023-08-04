//
//  Widget.hpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

namespace Kreator_UI
{
  using namespace IKan;
  
  // This class manage the Imgui predefined widget like Search, Setting etc
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

  private:
    inline static Ref<Image> s_searchIcon;
    inline static Ref<Image> s_gearIcon;
    inline static char* s_assetSearchString;
  }; 
} // namespace Kreator_UI
