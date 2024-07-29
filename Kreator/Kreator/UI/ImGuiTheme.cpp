//
//  ImGuiTheme.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ImGuiTheme.hpp"

namespace Kreator::UI
{
  void Color::SetTheme(Kreator::UserPreferences::Theme theme)
  {
    IK_PROFILE();
    
    bool setTheme = true;
    
    if (setTheme)
    {
      switch (theme)
      {
        case Kreator::UserPreferences::Theme::Blue:
        {
          // Text
          UI::Color::Text                   = IM_COL32(210, 210, 210, 255);
          UI::Color::TextMuted              = IM_COL32(110, 110, 110, 255);
          UI::Color::TextSelectedBg         = IM_COL32(30, 50, 70, 255);

          // Window
          UI::Color::Window                 = IM_COL32(100, 0, 0, 255);
          UI::Color::Background             = IM_COL32(100, 0, 0, 255);
          UI::Color::PopupBackground        = IM_COL32(20, 40, 60, 255);
          UI::Color::Border                 = PopupBackground;
          UI::Color::BackgroundDark         = IM_COL32(100, 0, 0, 255);
          
          // Models
          UI::Color::WindowDimBg            = PopupBackground;
          
          // Frame : Checkbox BG, Text BG, Drop Menu
          UI::Color::FrameBg                = IM_COL32(100, 0, 0, 255);
          UI::Color::FrameBgHovered         = IM_COL32(100, 0, 0, 255);
          UI::Color::FrameBgActive          = IM_COL32(100, 0, 0, 255);
          
          // Widget Title BG
          UI::Color::TitleBar               = IM_COL32(30, 50, 70, 255);
          UI::Color::TitleBarDark           = IM_COL32(100, 0, 0, 255);
          UI::Color::TitleBgActive          = IM_COL32(100, 0, 0, 255);
          UI::Color::TitleBgCollapsed       = IM_COL32(100, 0, 0, 255);
          
          // Title Menu BG
          UI::Color::MenuBarBg              = IM_COL32(100, 0, 0, 255);
          
          // Scrol Bar
          UI::Color::ScrollbarBg            = IM_COL32(100, 0, 0, 255);
          UI::Color::ScrollbarGrab          = IM_COL32(100, 0, 0, 255);
          UI::Color::ScrollbarGrabHovered   = IM_COL32(100, 0, 0, 255);
          UI::Color::ScrollbarGrabActive    = IM_COL32(100, 0, 0, 255);
          
          // Check Mark
          UI::Color::CheckMark              = IM_COL32(100, 0, 0, 255);
          
          // Slider
          UI::Color::SliderGrab             = IM_COL32(100, 0, 0, 255);
          UI::Color::SliderGrabActive       = IM_COL32(100, 0, 0, 255);
          
          // Button
          UI::Color::Button                 = IM_COL32(100, 0, 0, 255);
          UI::Color::ButtonHovered          = IM_COL32(100, 0, 0, 255);
          UI::Color::ButtonActive           = IM_COL32(100, 0, 0, 255);
          
          // Tree Node Headers
          UI::Color::Header                 = IM_COL32(100, 0, 0, 255);
          UI::Color::HeaderHovered          = IM_COL32(100, 0, 0, 255);
          UI::Color::HeaderActive           = IM_COL32(100, 0, 0, 255);
          
          // Widget and Column Separator
          UI::Color::Separator              = IM_COL32(50, 70, 90, 255);
          UI::Color::SeparatorActive        = IM_COL32(100, 0, 0, 255);
          UI::Color::SeparatorHovered       = IM_COL32(100, 0, 0, 255);
          
          // Resize
          UI::Color::ResizeGrip             = IM_COL32(100, 0, 0, 255);
          UI::Color::ResizeGripHovered      = IM_COL32(100, 0, 0, 255);
          UI::Color::ResizeGripActive       = IM_COL32(100, 0, 0, 255);
          
          // Widget Tab Tabs
          UI::Color::Tab                    = IM_COL32(100, 0, 0, 255);
          UI::Color::TabHovered             = IM_COL32(100, 0, 0, 255);
          UI::Color::TabActive              = IM_COL32(100, 0, 0, 255);
          UI::Color::TabUnfocused           = IM_COL32(100, 0, 0, 255);
          UI::Color::TabUnfocusedActive     = IM_COL32(100, 0, 0, 255);
          
          // Docking
          UI::Color::DockingPreview         = IM_COL32(100, 0, 0, 255);
          UI::Color::DockingEmptyBg         = IM_COL32(100, 0, 0, 255);
          
          // Plot
          UI::Color::PlotLines              = IM_COL32(100, 0, 0, 255);
          UI::Color::PlotLinesHovered       = IM_COL32(100, 0, 0, 255);
          UI::Color::PlotHistogram          = IM_COL32(100, 0, 0, 255);
          UI::Color::PlotHistogramHovered   = IM_COL32(100, 0, 0, 255);
          
          // Table
          UI::Color::TableHeaderBg          = IM_COL32(100, 0, 0, 255);
          UI::Color::TableBorderLight       = UI::Color::Separator;
          UI::Color::TableBorderStrong      = IM_COL32(100, 0, 0, 255);
          UI::Color::TableRowBg             = IM_COL32(100, 0, 0, 255);
          UI::Color::TableRowBgAlt          = IM_COL32(100, 0, 0, 255);
          
          // Drag Drop
          UI::Color::DragDropTarget         = IM_COL32(100, 0, 0, 255);
          
          // Navigation
          UI::Color::NavHighlight           = IM_COL32(100, 0, 0, 255);
          UI::Color::NavWindowingHighlight  = IM_COL32(100, 0, 0, 255);
          UI::Color::NavWindowingDimBg      = IM_COL32(100, 0, 0, 255);
          
          break;
        }
        default:
          break;
      }
    }
    
    ImVec4* colors = ImGui::GetStyle().Colors;
    
    // Text
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    colors[ImGuiCol_TextDisabled]           = ImGui::ColorConvertU32ToFloat4(UI::Color::TextMuted);
    colors[ImGuiCol_TextSelectedBg]         = ImGui::ColorConvertU32ToFloat4(UI::Color::TextSelectedBg);
    
    // Window
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4(UI::Color::Window);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::Background);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::PopupBackground);
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::Border);
    colors[ImGuiCol_BorderShadow]           = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    
    // Model
    colors[ImGuiCol_ModalWindowDimBg]       = ImGui::ColorConvertU32ToFloat4(UI::Color::WindowDimBg);
    
    // Frame : Checkbox BG, Text BG, Drop Menu
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::FrameBg);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4(UI::Color::FrameBgHovered);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::FrameBgActive);
    
    // Widget Title BG
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::TitleBar);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::TitleBgActive);
    colors[ImGuiCol_TitleBgCollapsed]       = ImGui::ColorConvertU32ToFloat4(UI::Color::TitleBgCollapsed);
    
    // Title Menu BG
    colors[ImGuiCol_MenuBarBg]              = ImGui::ColorConvertU32ToFloat4(UI::Color::MenuBarBg);
    
    // Scrol Bar
    colors[ImGuiCol_ScrollbarBg]            = ImGui::ColorConvertU32ToFloat4(UI::Color::ScrollbarBg);
    colors[ImGuiCol_ScrollbarGrab]          = ImGui::ColorConvertU32ToFloat4(UI::Color::ScrollbarGrab);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImGui::ColorConvertU32ToFloat4(UI::Color::ScrollbarGrabHovered);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImGui::ColorConvertU32ToFloat4(UI::Color::ScrollbarGrabActive);
    
    // Check Mark
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4(UI::Color::CheckMark);
    
    // Slider
    colors[ImGuiCol_SliderGrab]             = ImGui::ColorConvertU32ToFloat4(UI::Color::SliderGrab);
    colors[ImGuiCol_SliderGrabActive]       = ImGui::ColorConvertU32ToFloat4(UI::Color::SliderGrabActive);
    
    // Button
    colors[ImGuiCol_Button]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::Button);
    colors[ImGuiCol_ButtonHovered]          = ImGui::ColorConvertU32ToFloat4(UI::Color::ButtonHovered);
    colors[ImGuiCol_ButtonActive]           = ImGui::ColorConvertU32ToFloat4(UI::Color::ButtonActive);
    
    // Tree Node Headers
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::Header);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4(UI::Color::HeaderHovered);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4(UI::Color::HeaderActive);
    
    // Widget and Column Separator
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4(UI::Color::Separator);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4(UI::Color::SeparatorActive);
    colors[ImGuiCol_SeparatorHovered]       = ImGui::ColorConvertU32ToFloat4(UI::Color::SeparatorHovered);
    
    // Resize
    colors[ImGuiCol_ResizeGrip]             = ImGui::ColorConvertU32ToFloat4(UI::Color::ResizeGrip);
    colors[ImGuiCol_ResizeGripHovered]      = ImGui::ColorConvertU32ToFloat4(UI::Color::ResizeGripHovered);
    colors[ImGuiCol_ResizeGripActive]       = ImGui::ColorConvertU32ToFloat4(UI::Color::ResizeGripActive);
    
    // Widget Tab Tabs
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4(UI::Color::Tab);
    colors[ImGuiCol_TabHovered]             = ImGui::ColorConvertU32ToFloat4(UI::Color::TabHovered);
    colors[ImGuiCol_TabActive]              = ImGui::ColorConvertU32ToFloat4(UI::Color::TabActive);
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4(UI::Color::TabUnfocused);
    colors[ImGuiCol_TabUnfocusedActive]     = ImGui::ColorConvertU32ToFloat4(UI::Color::TabUnfocusedActive);
    
    // Docking
    colors[ImGuiCol_DockingPreview]         = ImGui::ColorConvertU32ToFloat4(UI::Color::DockingPreview);
    colors[ImGuiCol_DockingEmptyBg]         = ImGui::ColorConvertU32ToFloat4(UI::Color::DockingEmptyBg);
    
    // Plot
    colors[ImGuiCol_PlotLines]              = ImGui::ColorConvertU32ToFloat4(UI::Color::PlotLines);
    colors[ImGuiCol_PlotLinesHovered]       = ImGui::ColorConvertU32ToFloat4(UI::Color::PlotLinesHovered);
    colors[ImGuiCol_PlotHistogram]          = ImGui::ColorConvertU32ToFloat4(UI::Color::PlotHistogram);
    colors[ImGuiCol_PlotHistogramHovered]   = ImGui::ColorConvertU32ToFloat4(UI::Color::PlotHistogramHovered);
    
    // Table
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4(UI::Color::TableHeaderBg);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4(UI::Color::TableBorderLight);
    colors[ImGuiCol_TableBorderStrong]      = ImGui::ColorConvertU32ToFloat4(UI::Color::TableBorderStrong);
    colors[ImGuiCol_TableRowBg]             = ImGui::ColorConvertU32ToFloat4(UI::Color::TableRowBg);
    colors[ImGuiCol_TableRowBgAlt]          = ImGui::ColorConvertU32ToFloat4(UI::Color::TableRowBgAlt);
    
    // Drag Drop
    colors[ImGuiCol_DragDropTarget]         = ImGui::ColorConvertU32ToFloat4(UI::Color::DragDropTarget);
    
    // Navigation
    colors[ImGuiCol_NavHighlight]           = ImGui::ColorConvertU32ToFloat4(UI::Color::NavHighlight);
    colors[ImGuiCol_NavWindowingHighlight]  = ImGui::ColorConvertU32ToFloat4(UI::Color::NavWindowingHighlight);
    colors[ImGuiCol_NavWindowingDimBg]      = ImGui::ColorConvertU32ToFloat4(UI::Color::NavWindowingDimBg);
  }
  
  glm::vec4 Color::Vec3FromU32(const ImU32& color)
  {
    ImVec4 result = ImGui::ColorConvertU32ToFloat4(color);
    return {result.x, result.y, result.z, result.w};
  }
  ImU32 Color::U32FromVec3(const glm::vec4& color)
  {
    return ImGui::ColorConvertFloat4ToU32({color.r, color.g, color.b, color.a});
  }
  
  void Font::Load(const std::unordered_map<FontType, IKan::UI::ImGuiFont>& otherFonts)
  {
    IK_PROFILE();
    
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.OnDetach();
    imguiLayer.OnAttach();
    
    // Store the fonts in vector in same order which we need to pass to ImGui
    std::vector <IKan::UI::ImGuiFont> fonts(otherFonts.size());
    for (uint8_t fontIdx = 0; fontIdx < (uint8_t)FontType::Max; fontIdx++)
    {
      fonts[fontIdx] = otherFonts.at((FontType)fontIdx);
    }
    
    imguiLayer.LoadFonts(fonts);
  }
  
  ImFont* Font::Get(FontType type)
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[static_cast<int32_t>(type)] : fonts[0];
  }
} // namespace Kreator::UI
