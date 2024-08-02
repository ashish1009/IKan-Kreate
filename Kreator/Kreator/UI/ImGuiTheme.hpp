//
//  ImGuiTheme.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Editor/UserPreference.hpp"

namespace Kreator::UI
{
  /// This structure stores the ImGui Color values
  struct Color
  {
    // Text
    inline static ImU32 Text                   = IM_COL32(200, 200, 200, 255);
    inline static ImU32 TextMuted              = IM_COL32(128, 128, 128, 255);
    inline static ImU32 TextBrighter           = IM_COL32(255, 255, 255, 255);
    inline static ImU32 TextSelectedBg         = IM_COL32(64, 140, 240, 255);
    
    // Window
    inline static ImU32 Window                 = IM_COL32(25, 25, 25, 255);
    inline static ImU32 Background             = IM_COL32(35, 35, 35, 255);
    inline static ImU32 PopupBackground        = IM_COL32(20, 20, 20, 255);
    inline static ImU32 Border                 = IM_COL32(110, 110, 128, 128);
    inline static ImU32 BackgroundDark         = IM_COL32(15, 15, 15, 0);
    
    // Models
    inline static ImU32 WindowDimBg            = IM_COL32(20, 20, 20, 255);
    
    // Frame : Checkbox BG, Text BG, Drop Menu
    inline static ImU32 FrameBg                = IM_COL32(40, 74, 122, 130);
    inline static ImU32 FrameBgHovered         = IM_COL32(70, 240, 240, 120);
    inline static ImU32 FrameBgActive          = IM_COL32(70, 240, 240, 150);
    
    // Widget Title BG
    inline static ImU32 TitleBar               = IM_COL32(10, 10, 10, 255);
    inline static ImU32 TitleBarDark           = IM_COL32(5, 5, 5, 155);
    inline static ImU32 TitleBgActive          = IM_COL32(40, 80, 125, 255);
    inline static ImU32 TitleBgCollapsed       = IM_COL32(220, 0, 0, 128);
    
    // Title Menu BG
    inline static ImU32 MenuBarBg              = IM_COL32(35, 35, 35, 255);
    
    // Scrol Bar
    inline static ImU32 ScrollbarBg            = IM_COL32(5, 5, 5, 128);
    inline static ImU32 ScrollbarGrab          = IM_COL32(80, 80, 80, 255);
    inline static ImU32 ScrollbarGrabHovered   = IM_COL32(100, 100, 100, 255);
    inline static ImU32 ScrollbarGrabActive    = IM_COL32(130, 130, 130, 255);
    
    // Check Mark
    inline static ImU32 CheckMark              = IM_COL32(68, 140, 240, 255);
    
    // Slider
    inline static ImU32 SliderGrab             = IM_COL32(62, 130, 220, 255);
    inline static ImU32 SliderGrabActive       = IM_COL32(72, 140, 240, 255);
    
    // Button
    inline static ImU32 Button                 = IM_COL32(68, 140, 240, 120);
    inline static ImU32 ButtonHovered          = IM_COL32(68, 140, 240, 255);
    inline static ImU32 ButtonActive           = IM_COL32(15, 160, 240, 255);
    
    // Tree Node Headers
    inline static ImU32 Header                 = IM_COL32(68, 140, 240, 75);
    inline static ImU32 HeaderHovered          = IM_COL32(68, 140, 240, 200);
    inline static ImU32 HeaderActive           = IM_COL32(68, 140, 240, 255);
    
    // Widget and Column Separator
    inline static ImU32 Separator              = Border;
    inline static ImU32 SeparatorActive        = IM_COL32(25, 60, 180, 190);
    inline static ImU32 SeparatorHovered       = IM_COL32(25, 60, 180, 255);
    
    // Resize
    inline static ImU32 ResizeGrip             = IM_COL32(64, 140, 240, 50);
    inline static ImU32 ResizeGripHovered      = IM_COL32(64, 140, 240, 150);
    inline static ImU32 ResizeGripActive       = IM_COL32(64, 140, 240, 250);
    
    // Widget Tab Tabs
    inline static ImU32 Tab                    = ImGui::ColorConvertFloat4ToU32(ImLerp(ImGui::ColorConvertU32ToFloat4(Header), ImGui::ColorConvertU32ToFloat4(TitleBgActive), 0.80f));
    inline static ImU32 TabHovered             = HeaderHovered;
    inline static ImU32 TabActive              = ImGui::ColorConvertFloat4ToU32(ImLerp(ImGui::ColorConvertU32ToFloat4(HeaderActive), ImGui::ColorConvertU32ToFloat4(TitleBgActive), 0.80f));
    inline static ImU32 TabUnfocused           = ImGui::ColorConvertFloat4ToU32(ImLerp(ImGui::ColorConvertU32ToFloat4(Tab), ImGui::ColorConvertU32ToFloat4(TitleBar), 0.80f));
    inline static ImU32 TabUnfocusedActive     = ImGui::ColorConvertFloat4ToU32(ImLerp(ImGui::ColorConvertU32ToFloat4(TabActive), ImGui::ColorConvertU32ToFloat4(TitleBar), 0.80f));
    
    // Docking
    inline static ImU32 DockingPreview         = HeaderActive;
    inline static ImU32 DockingEmptyBg         = IM_COL32(50, 50, 50, 255);
    
    // Plot
    inline static ImU32 PlotLines              = IM_COL32(155, 155, 055, 255);
    inline static ImU32 PlotLinesHovered       = IM_COL32(255, 120, 90, 255);
    inline static ImU32 PlotHistogram          = IM_COL32(250, 200, 0, 255);
    inline static ImU32 PlotHistogramHovered   = IM_COL32(255, 160, 0, 255);
    
    // Table
    inline static ImU32 TableHeaderBg          = IM_COL32(50, 50, 50, 255);
    inline static ImU32 TableBorderStrong      = IM_COL32(78, 78, 85, 255);
    inline static ImU32 TableBorderLight       = IM_COL32(55, 55, 60, 255);
    inline static ImU32 TableRowBg             = IM_COL32(0, 0, 0, 0);
    inline static ImU32 TableRowBgAlt          = IM_COL32(255, 255, 255, 15);
    
    // Drag Drop
    inline static ImU32 DragDropTarget         = IM_COL32(255, 255, 255, 245);
    
    // Navigation
    inline static ImU32 NavHighlight           = IM_COL32(68, 140, 240, 255);
    inline static ImU32 NavWindowingHighlight  = IM_COL32(255, 255, 255, 200);
    inline static ImU32 NavWindowingDimBg      = IM_COL32(200, 200, 200, 55);
    
    // Other
    inline static ImU32 NiceBlueHighlight     = IM_COL32(83, 232, 254, 255);
    inline static ImU32 NiceRedHighlight      = IM_COL32(210, 45, 35, 255);
    inline static ImU32 Accent                = IM_COL32(236, 158, 36, 255);
    inline static ImU32 HoveredItem           = IM_COL32(136, 100, 20, 80);
    inline static ImU32 Warning               = IM_COL32(183, 32, 54, 255);
    inline static ImU32 Selection             = IM_COL32(191, 177, 155, 255);
    inline static ImU32 SelectionMuted        = IM_COL32(59, 57, 45, 255);
    inline static ImU32 TextDarker            = IM_COL32(158, 158, 158, 255);
    inline static ImU32 PropertyField         = IM_COL32(25, 35, 45, 255);
    inline static ImU32 GroupHeader           = IM_COL32(27, 27, 27, 255);
    inline static ImU32 GroupHeaderHovered    = IM_COL32(32, 32, 32, 255);
    inline static ImU32 GroupHeaderActive     = IM_COL32(30, 30, 30, 255);
    inline static ImU32 Muted                 = IM_COL32(77, 77, 77, 255);
    inline static ImU32 TextError             = IM_COL32(230, 51, 51, 255);

    /// This function changes theme color to dark
    /// - Parameter theme: Current selected theme
    static void SetTheme(Kreator::UserPreferences::Theme theme);

    /// This function converts the ImU32 color to Vec4
    /// - Parameter color: U32 color
    static glm::vec4 Vec3FromU32(const ImU32& color);
    /// This function converts the ImU32 color from Vec4
    /// - Parameter color: vec4 coloe
    static ImU32 U32FromVec3(const glm::vec4& color);
  };
  
  /// This enum stores the fonts of Kreator
  enum class FontType : uint8_t
  {
    Regular, HugeHeader, SemiHeader, Bold, Italic, FixedWidth,
    Max // Should be at last for loop in class
  };
  
  /// This class provides the wrapper APIs to load and use the ImGui fonts
  class Font
  {
  public:
    /// This function changes the imgui font
    ///  - Parameter otherFonts: other fonts
    static void Load(const std::unordered_map<FontType, IKan::UI::ImGuiFont>& otherFonts);
    /// This function returns the font from type enum
    /// - Parameter type: Font type
    static ImFont* Get(FontType type);
  };
} // namespace Kreator::UI
