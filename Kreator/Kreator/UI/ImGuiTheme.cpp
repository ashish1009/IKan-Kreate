//
//  ImGuiTheme.cpp
//  Kreator
//
//  Created by Ashish . on 15/02/24.
//

#include "ImGuiTheme.hpp"
namespace Kreator::UI
{
  void SetThemeColors(Kreator::UserPreferences::Theme theme)
  {
    IK_PROFILE();
    
    switch (theme)
    {
      case Kreator::UserPreferences::Theme::Dark:
        UI::Color::Text                 = IM_COL32(222, 222, 222, 255);
        UI::Color::TextDarker           = IM_COL32(128, 128, 128, 255);
        UI::Color::TextBrighter         = IM_COL32(210, 210, 210, 255);
        UI::Color::Muted                = IM_COL32(77, 77, 77, 255);
        UI::Color::Background           = IM_COL32(28, 28, 28, 255);
        UI::Color::BackgroundPopup      = IM_COL32(39, 39, 39, 255);
        UI::Color::BackgroundDark       = IM_COL32(21, 21, 21, 255);
        UI::Color::WindowDimBg          = IM_COL32(25, 25, 25, 255);
        UI::Color::Titlebar             = IM_COL32(31, 31, 31, 255);
        UI::Color::TitleBarDark         = IM_COL32(21, 21, 21, 255);
        UI::Color::ActiveTab            = IM_COL32(71, 71, 71, 255);
        UI::Color::PropertyField        = IM_COL32(25, 25, 25, 255);
        UI::Color::GroupHeader          = IM_COL32(27, 27, 27, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(32, 32, 32, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(30, 30, 30, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(83, 83, 83, 255);
        break;
        
      case Kreator::UserPreferences::Theme::Blue:
        UI::Color::Text                 = IM_COL32(222, 222, 222, 255);
        UI::Color::TextDarker           = IM_COL32(128, 128, 128, 255);
        UI::Color::TextBrighter         = IM_COL32(210, 210, 210, 255);
        UI::Color::Muted                = IM_COL32(97, 97, 97, 255);
        UI::Color::Background           = IM_COL32(28, 38, 53, 255);
        UI::Color::BackgroundPopup      = IM_COL32(33, 53, 71, 255);
        UI::Color::BackgroundDark       = IM_COL32(21, 31, 51, 255);
        UI::Color::WindowDimBg          = IM_COL32(15, 25, 45, 255);
        UI::Color::Titlebar             = IM_COL32(35, 45, 60, 255);
        UI::Color::TitleBarDark         = IM_COL32(25, 35, 50, 255);
        UI::Color::ActiveTab            = IM_COL32(71, 71, 95, 255);
        UI::Color::PropertyField        = IM_COL32(25, 35, 45, 255);
        UI::Color::GroupHeader          = IM_COL32(27, 37, 57, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(32, 42, 62, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(30, 40, 60, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(83, 232, 254, 255);
        break;
        
      case Kreator::UserPreferences::Theme::Light:
        UI::Color::Text                 = IM_COL32(35, 35, 35, 255);
        UI::Color::TextDarker           = IM_COL32(22, 22, 22, 255);
        UI::Color::TextBrighter         = IM_COL32(60, 60, 60, 255);
        UI::Color::Muted                = IM_COL32(77, 77, 77, 255);
        UI::Color::Background           = IM_COL32(168, 168, 163, 255);
        UI::Color::BackgroundPopup      = IM_COL32(143, 143, 143, 255);
        UI::Color::BackgroundDark       = IM_COL32(121, 121, 121, 255);
        UI::Color::WindowDimBg          = IM_COL32(115, 115, 115, 255);
        UI::Color::Titlebar             = IM_COL32(135, 135, 135, 255);
        UI::Color::TitleBarDark         = IM_COL32(125, 125, 125, 255);
        UI::Color::ActiveTab            = IM_COL32(171, 171, 171, 255);
        UI::Color::PropertyField        = IM_COL32(125, 125, 125, 255);
        UI::Color::GroupHeader          = IM_COL32(127, 127, 127, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(132, 132, 132, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(130, 130, 130, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(183, 183, 183, 255);
        break;
        
      default:
        break;
    }
    
    ImVec4* colors = ImGui::GetStyle().Colors;
    // Text
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    colors[ImGuiCol_TextDisabled]           = ImGui::ColorConvertU32ToFloat4(UI::Color::Muted);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    
    // Window
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::Background);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundPopup);
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    
    // Borders
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    
    // Frame : Checkbox BG, Text BG, Drop Menue BG
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    
    // Widget Title BG
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    
    // Title Menu BG
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    
    // Scrol Bar
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    
    // Check Mark
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    
    // Slider
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
    
    // Button
    colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
    
    // Tree Node Headers
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeader);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeaderHovered);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeaderActive);
    
    // Widget and Column Separator
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4(UI::Color::Highlight);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.16f, 0.16f, 0.16f, 0.78f);
    
    // Resize
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    
    // Widget Tab Tabs
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TabHovered]             = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundPopup);
    colors[ImGuiCol_TabActive]              = ImGui::ColorConvertU32ToFloat4(UI::Color::ActiveTab);
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TabUnfocusedActive]     = colors[ImGuiCol_TabHovered];
    
    // Docking
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);
    
    // Plot
    colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.0f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.0f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(0.00f, 1.60f, 0.00f, 1.0f);
    
    // Table
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeader);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    colors[ImGuiCol_TableRowBg]             = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.2f, 0.2f, 0.3f, 1.0f);
    
    // Drag Drop
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
    
    // Navigation
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.0f, 0.0f, 0.7f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.0f, 0.0f, 1.0f);
    
    // Model
    colors[ImGuiCol_ModalWindowDimBg]       = ImGui::ColorConvertU32ToFloat4(UI::Color::WindowDimBg);
  }
  
  void LoadFonts(const std::vector<IKan::UI::ImGuiFont>& otherFonts)
  {
    IK_PROFILE();
    auto& imguiLayer = Application::Get().GetImGuiLayer();
    imguiLayer.OnDetach();
    
    imguiLayer.OnAttach();
    imguiLayer.LoadFonts(otherFonts);
  }
  
  ImFont* GetRegularFont()
  {
    return ImGui::GetIO().Fonts->Fonts[0];
  }
  ImFont* GetBoldFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[1] : fonts[0];
  }
  ImFont* GetItalicFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[2] : fonts[0];
  }
  ImFont* GetFixedWidthFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[3] : fonts[0];
  }
  ImFont* GetHugeHeaderFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[4] : fonts[0];
  }
  ImFont* GetSemiHeaderFont()
  {
    auto fonts = ImGui::GetIO().Fonts->Fonts;
    return fonts.size() > 1 ? fonts[5] : fonts[0];
  }
} // Kreator::UI
