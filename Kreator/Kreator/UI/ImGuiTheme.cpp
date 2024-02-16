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
        UI::Color::Text                 = IM_COL32(255, 0, 0, 255);
        UI::Color::TextDarker           = IM_COL32(255, 0, 0, 255);
        UI::Color::TextBrighter         = IM_COL32(255, 0, 0, 255);
        UI::Color::Muted                = IM_COL32(255, 0, 0, 255);
        UI::Color::Background           = IM_COL32(255, 0, 0, 255);
        UI::Color::BackgroundPopup      = IM_COL32(43, 43, 43, 255);
        UI::Color::BackgroundDark       = IM_COL32(255, 0, 0, 255);
        UI::Color::WindowDimBg          = IM_COL32(25, 25, 25, 255);
        UI::Color::Titlebar             = IM_COL32(255, 0, 0, 255);
        UI::Color::TitleBarDark         = IM_COL32(255, 0, 0, 255);
        UI::Color::ActiveTab            = IM_COL32(255, 0, 0, 255);
        UI::Color::PropertyField        = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeader          = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(255, 0, 0, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(255, 0, 0, 255);
        break;
        
      case Kreator::UserPreferences::Theme::Blue:
        UI::Color::Text                 = IM_COL32(255, 0, 0, 255);
        UI::Color::TextDarker           = IM_COL32(255, 0, 0, 255);
        UI::Color::TextBrighter         = IM_COL32(255, 0, 0, 255);
        UI::Color::Muted                = IM_COL32(255, 0, 0, 255);
        UI::Color::Background           = IM_COL32(255, 0, 0, 255);
        UI::Color::BackgroundPopup      = IM_COL32(43, 63, 91, 255);
        UI::Color::BackgroundDark       = IM_COL32(255, 0, 0, 255);
        UI::Color::WindowDimBg          = IM_COL32(15, 25, 45, 255);
        UI::Color::Titlebar             = IM_COL32(255, 0, 0, 255);
        UI::Color::TitleBarDark         = IM_COL32(255, 0, 0, 255);
        UI::Color::ActiveTab            = IM_COL32(255, 0, 0, 255);
        UI::Color::PropertyField        = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeader          = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(255, 0, 0, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(255, 0, 0, 255);
        break;
        
      case Kreator::UserPreferences::Theme::Light:
        UI::Color::Text                 = IM_COL32(255, 0, 0, 255);
        UI::Color::TextDarker           = IM_COL32(255, 0, 0, 255);
        UI::Color::TextBrighter         = IM_COL32(255, 0, 0, 255);
        UI::Color::Muted                = IM_COL32(255, 0, 0, 255);
        UI::Color::Background           = IM_COL32(255, 0, 0, 255);
        UI::Color::BackgroundPopup      = IM_COL32(143, 143, 143, 255);
        UI::Color::BackgroundDark       = IM_COL32(255, 0, 0, 255);
        UI::Color::WindowDimBg          = IM_COL32(115, 115, 115, 255);
        UI::Color::Titlebar             = IM_COL32(255, 0, 0, 255);
        UI::Color::TitleBarDark         = IM_COL32(255, 0, 0, 255);
        UI::Color::ActiveTab            = IM_COL32(255, 0, 0, 255);
        UI::Color::PropertyField        = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeader          = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderHovered   = IM_COL32(255, 0, 0, 255);
        UI::Color::GroupHeaderActive    = IM_COL32(255, 0, 0, 255);
        UI::Color::NiceThemeHighlight   = IM_COL32(255, 0, 0, 255);
        break;
        
      default:
        break;
    }
    
    ImVec4* colors = ImGui::GetStyle().Colors;
    // Text
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    colors[ImGuiCol_TextDisabled]           = ImGui::ColorConvertU32ToFloat4(UI::Color::Muted);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(1, 0, 0, 1);
    
    // Window
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::Background);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundPopup);
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    
    // Borders
    colors[ImGuiCol_Border]                 = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_BorderShadow]           = ImVec4(1, 0, 0, 1);
    
    // Frame : Checkbox BG, Text BG, Drop Menue BG
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::PropertyField);
    
    // Widget Title BG
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(1, 0, 0, 1);
    
    // Title Menu BG
    colors[ImGuiCol_MenuBarBg]              = ImVec4(1, 0, 0, 1);
    
    // Scrol Bar
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(1, 0, 0, 1);
    
    // Check Mark
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4(UI::Color::Text);
    
    // Slider
    colors[ImGuiCol_SliderGrab]             = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(1, 0, 0, 1);
    
    // Button
    colors[ImGuiCol_Button]                 = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_ButtonActive]           = ImVec4(1, 0, 0, 1);
    
    // Tree Node Headers
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeader);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeaderHovered);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeaderActive);
    
    // Widget and Column Separator
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4(UI::Color::Highlight);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(1, 0, 0, 1);
    
    // Resize
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.01f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.01f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.01f);

    // Widget Tab Tabs
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TabHovered]             = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundPopup);
    colors[ImGuiCol_TabActive]              = ImGui::ColorConvertU32ToFloat4(UI::Color::ActiveTab);
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TabUnfocusedActive]     = colors[ImGuiCol_TabHovered];
    
    // Docking
    colors[ImGuiCol_DockingPreview]         = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1, 0, 0, 1);
    
    // Plot
    colors[ImGuiCol_PlotLines]              = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1, 0, 0, 1);
    
    // Table
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4(UI::Color::GroupHeader);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4(UI::Color::BackgroundDark);
    colors[ImGuiCol_TableRowBg]             = ImGui::ColorConvertU32ToFloat4(UI::Color::Titlebar);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1, 0, 0, 1);
    
    // Drag Drop
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1, 0, 0, 1);
    
    // Navigation
    colors[ImGuiCol_NavHighlight]           = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1, 0, 0, 1);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1, 0, 0, 1);
    
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
