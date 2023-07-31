//
//  ImGuiTheme.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiTheme.hpp"

namespace IKan::UI
{
  void Theme::Color::Update()
  {
    ImVec4* colors = ImGui::GetStyle().Colors;
    
    colors[ImGuiCol_Text]                   = ImGui::ColorConvertU32ToFloat4(Color::Text);
    colors[ImGuiCol_TextDisabled]           = ImGui::ColorConvertU32ToFloat4(Color::TextDisabled);
    colors[ImGuiCol_WindowBg]               = ImGui::ColorConvertU32ToFloat4(Color::WindowBg);
    colors[ImGuiCol_ChildBg]                = ImGui::ColorConvertU32ToFloat4(Color::ChildBg);
    colors[ImGuiCol_PopupBg]                = ImGui::ColorConvertU32ToFloat4(Color::PopupBg);
    colors[ImGuiCol_Border]                 = ImGui::ColorConvertU32ToFloat4(Color::Border);
    colors[ImGuiCol_BorderShadow]           = ImGui::ColorConvertU32ToFloat4(Color::BorderShadow);
    colors[ImGuiCol_FrameBg]                = ImGui::ColorConvertU32ToFloat4(Color::FrameBg);
    colors[ImGuiCol_FrameBgHovered]         = ImGui::ColorConvertU32ToFloat4(Color::FrameBgHovered);
    colors[ImGuiCol_FrameBgActive]          = ImGui::ColorConvertU32ToFloat4(Color::FrameBgActive);
    colors[ImGuiCol_TitleBg]                = ImGui::ColorConvertU32ToFloat4(Color::TitleBg);
    colors[ImGuiCol_TitleBgActive]          = ImGui::ColorConvertU32ToFloat4(Color::TitleBgActive);
    colors[ImGuiCol_TitleBgCollapsed]       = ImGui::ColorConvertU32ToFloat4(Color::TitleBgCollapsed);
    colors[ImGuiCol_MenuBarBg]              = ImGui::ColorConvertU32ToFloat4(Color::MenuBarBg);
    colors[ImGuiCol_ScrollbarBg]            = ImGui::ColorConvertU32ToFloat4(Color::ScrollbarBg);
    colors[ImGuiCol_ScrollbarGrab]          = ImGui::ColorConvertU32ToFloat4(Color::ScrollbarGrab);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImGui::ColorConvertU32ToFloat4(Color::ScrollbarGrabHovered);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImGui::ColorConvertU32ToFloat4(Color::ScrollbarGrabActive);
    colors[ImGuiCol_CheckMark]              = ImGui::ColorConvertU32ToFloat4(Color::CheckMark);
    colors[ImGuiCol_SliderGrab]             = ImGui::ColorConvertU32ToFloat4(Color::SliderGrab);
    colors[ImGuiCol_SliderGrabActive]       = ImGui::ColorConvertU32ToFloat4(Color::SliderGrabActive);
    colors[ImGuiCol_Button]                 = ImGui::ColorConvertU32ToFloat4(Color::Button);
    colors[ImGuiCol_ButtonHovered]          = ImGui::ColorConvertU32ToFloat4(Color::ButtonHovered);
    colors[ImGuiCol_ButtonActive]           = ImGui::ColorConvertU32ToFloat4(Color::ButtonActive);
    colors[ImGuiCol_Header]                 = ImGui::ColorConvertU32ToFloat4(Color::Header);
    colors[ImGuiCol_HeaderHovered]          = ImGui::ColorConvertU32ToFloat4(Color::HeaderHovered);
    colors[ImGuiCol_HeaderActive]           = ImGui::ColorConvertU32ToFloat4(Color::HeaderActive);
    colors[ImGuiCol_Separator]              = ImGui::ColorConvertU32ToFloat4(Color::Separator);
    colors[ImGuiCol_SeparatorHovered]       = ImGui::ColorConvertU32ToFloat4(Color::SeparatorHovered);
    colors[ImGuiCol_SeparatorActive]        = ImGui::ColorConvertU32ToFloat4(Color::SeparatorActive);
    colors[ImGuiCol_ResizeGrip]             = ImGui::ColorConvertU32ToFloat4(Color::ResizeGrip);
    colors[ImGuiCol_ResizeGripHovered]      = ImGui::ColorConvertU32ToFloat4(Color::ResizeGripHovered);
    colors[ImGuiCol_ResizeGripActive]       = ImGui::ColorConvertU32ToFloat4(Color::ResizeGripActive);
    colors[ImGuiCol_Tab]                    = ImGui::ColorConvertU32ToFloat4(Color::Tab);
    colors[ImGuiCol_TabHovered]             = ImGui::ColorConvertU32ToFloat4(Color::TabHovered);
    colors[ImGuiCol_TabActive]              = ImGui::ColorConvertU32ToFloat4(Color::TabActive);
    colors[ImGuiCol_TabUnfocused]           = ImGui::ColorConvertU32ToFloat4(Color::TabUnfocused);
    colors[ImGuiCol_TabUnfocusedActive]     = ImGui::ColorConvertU32ToFloat4(Color::TabUnfocusedActive);
    colors[ImGuiCol_DockingPreview]         = ImGui::ColorConvertU32ToFloat4(Color::DockingPreview);
    colors[ImGuiCol_DockingEmptyBg]         = ImGui::ColorConvertU32ToFloat4(Color::DockingEmptyBg);
    colors[ImGuiCol_PlotLines]              = ImGui::ColorConvertU32ToFloat4(Color::PlotLines);
    colors[ImGuiCol_PlotLinesHovered]       = ImGui::ColorConvertU32ToFloat4(Color::PlotLinesHovered);
    colors[ImGuiCol_PlotHistogram]          = ImGui::ColorConvertU32ToFloat4(Color::PlotHistogram);
    colors[ImGuiCol_PlotHistogramHovered]   = ImGui::ColorConvertU32ToFloat4(Color::PlotHistogramHovered);
    colors[ImGuiCol_TableHeaderBg]          = ImGui::ColorConvertU32ToFloat4(Color::TableHeaderBg);
    colors[ImGuiCol_TableBorderStrong]      = ImGui::ColorConvertU32ToFloat4(Color::TableBorderStrong);
    colors[ImGuiCol_TableBorderLight]       = ImGui::ColorConvertU32ToFloat4(Color::TableBorderLight);
    colors[ImGuiCol_TableRowBg]             = ImGui::ColorConvertU32ToFloat4(Color::TableRowBg);
    colors[ImGuiCol_TableRowBgAlt]          = ImGui::ColorConvertU32ToFloat4(Color::TableRowBgAlt);
    colors[ImGuiCol_TextSelectedBg]         = ImGui::ColorConvertU32ToFloat4(Color::TextSelectedBg);
    colors[ImGuiCol_DragDropTarget]         = ImGui::ColorConvertU32ToFloat4(Color::DragDropTarget);
    colors[ImGuiCol_NavHighlight]           = ImGui::ColorConvertU32ToFloat4(Color::NavHighlight);
    colors[ImGuiCol_NavWindowingHighlight]  = ImGui::ColorConvertU32ToFloat4(Color::NavWindowingHighlight);
    colors[ImGuiCol_NavWindowingDimBg]      = ImGui::ColorConvertU32ToFloat4(Color::NavWindowingDimBg);
    colors[ImGuiCol_ModalWindowDimBg]       = ImGui::ColorConvertU32ToFloat4(Color::ModalWindowDimBg);
  }
} // namespace IKan::UI
