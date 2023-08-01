//
//  Theme.cpp
//  Kreator
//
//  Created by Ashish . on 31/07/23.
//

#include "Theme.hpp"

namespace Kreator_UI
{
  void SetDarkTheme()
  {
    IKan::UI::Theme::Color::Text                 = IM_COL32(222, 222, 222, 255);
    IKan::UI::Theme::Color::Titlebar             = IM_COL32(45, 45, 60, 255);
    IKan::UI::Theme::Color::Background           = IM_COL32(28, 28, 33, 255);
    IKan::UI::Theme::Color::BackgroundPopup      = IM_COL32(63, 73, 87, 255);
    IKan::UI::Theme::Color::BackgroundDark       = IM_COL32(21, 21, 21, 255);
    IKan::UI::Theme::Color::PropertyField        = IM_COL32(15, 15, 15, 255);
    IKan::UI::Theme::Color::GroupHeader          = IM_COL32(37, 37, 37, 255);
    IKan::UI::Theme::Color::GroupHeaderHovered   = IM_COL32(32, 32, 32, 255);
    IKan::UI::Theme::Color::GroupHeaderActive    = IM_COL32(30, 30, 30, 255);
    IKan::UI::Theme::Color::TextBrighter         = IM_COL32(210, 210, 210, 255);
    
    ImVec4* colors = ImGui::GetStyle().Colors;
    
    // Text
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Text);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    
    // Window
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Titlebar);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Background);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::BackgroundPopup);
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::BackgroundDark);
    
    // Borders
    colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.5f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.0f);
    
    // Frame : Checkbox BG, Text BG, Drop Menue BG
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::PropertyField);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::PropertyField);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::PropertyField);
    
    // Widget Title BG
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Titlebar);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Titlebar);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    
    // Title Menu BG
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
    
    // Scrol Bar
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    
    // Check Mark
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Text);
    
    // Slider
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
    
    // Button
    colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.4f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.0f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.0f);
    
    // Tree Node Headers
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::GroupHeader);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::GroupHeaderHovered);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::GroupHeaderActive);
    
    // Widget and Column Separator
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::BackgroundDark);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4(Kreator_UI::Color::Highlight);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.16f, 0.16f, 0.16f, 0.78f);
    
    // Resize
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    
    // Widget Tab Tabs
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Titlebar);
    colors[ImGuiCol_TabHovered]             = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive]              = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::Titlebar);
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
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::GroupHeader);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4(IKan::UI::Theme::Color::BackgroundDark);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(1.0f, 0.0, 0.0, 1.0f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(1.0f, 0.0, 0.0, 1.0f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.0f, 0.0, 0.0, 1.0f);
    
    // Drag Drop
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
    
    // Navigation
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.6f, 0.6f, 1.0f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.0f, 0.0f, 0.7f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.0f, 0.0f, 1.0f);
    
    // Model
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.15f, 0.15f, 0.25f, 1.7f);
  }

  ImFont* GetRegularFont()
  {
    return ImGui::GetIO().Fonts->Fonts[0];
  }
  ImFont* GetBoldFont()
  {
    return ImGui::GetIO().Fonts->Fonts[1];
  }
  ImFont* GetItalicFont()
  {
    return ImGui::GetIO().Fonts->Fonts[2];
  }
  ImFont* GetFixedWidthFont()
  {
    return ImGui::GetIO().Fonts->Fonts[3];
  }
  ImFont* GetHugeHeaderFont()
  {
    return ImGui::GetIO().Fonts->Fonts[4];
  }

} // namespace Kreator_UI

