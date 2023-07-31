//
//  ImGuiTheme.hpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace IKan::UI
{
  struct Theme
  {
    // Default Theme Color -------------------------------------------------------------------------------------------
    struct Color
    {
      inline static ImU32 Text                   = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
      inline static ImU32 TextDisabled           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.50f, 0.50f, 0.50f, 1.00f));
      inline static ImU32 TextSelectedBg         = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.35f));
      inline static ImU32 WindowBg               = ImGui::ColorConvertFloat4ToU32(ImVec4(0.06f, 0.06f, 0.06f, 0.94f));
      inline static ImU32 ChildBg                = ImGui::ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
      inline static ImU32 PopupBg                = ImGui::ColorConvertFloat4ToU32(ImVec4(0.08f, 0.08f, 0.08f, 0.94f));
      inline static ImU32 Border                 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.43f, 0.43f, 0.50f, 0.50f));
      inline static ImU32 BorderShadow           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
      inline static ImU32 FrameBg                = ImGui::ColorConvertFloat4ToU32(ImVec4(0.16f, 0.29f, 0.48f, 0.54f));
      inline static ImU32 FrameBgHovered         = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
      inline static ImU32 FrameBgActive          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
      inline static ImU32 TitleBg                = ImGui::ColorConvertFloat4ToU32(ImVec4(0.04f, 0.04f, 0.04f, 1.00f));
      inline static ImU32 TitleBgActive          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.16f, 0.29f, 0.48f, 1.00f));
      inline static ImU32 TitleBgCollapsed       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 0.51f));
      inline static ImU32 MenuBarBg              = ImGui::ColorConvertFloat4ToU32(ImVec4(0.14f, 0.14f, 0.14f, 1.00f));
      inline static ImU32 ScrollbarBg            = ImGui::ColorConvertFloat4ToU32(ImVec4(0.02f, 0.02f, 0.02f, 0.53f));
      inline static ImU32 ScrollbarGrab          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.31f, 0.31f, 0.31f, 1.00f));
      inline static ImU32 ScrollbarGrabHovered   = ImGui::ColorConvertFloat4ToU32(ImVec4(0.41f, 0.41f, 0.41f, 1.00f));
      inline static ImU32 ScrollbarGrabActive    = ImGui::ColorConvertFloat4ToU32(ImVec4(0.51f, 0.51f, 0.51f, 1.00f));
      inline static ImU32 CheckMark              = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
      inline static ImU32 SliderGrab             = ImGui::ColorConvertFloat4ToU32(ImVec4(0.24f, 0.52f, 0.88f, 1.00f));
      inline static ImU32 SliderGrabActive       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
      inline static ImU32 Button                 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
      inline static ImU32 ButtonHovered          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
      inline static ImU32 ButtonActive           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.06f, 0.53f, 0.98f, 1.00f));
      inline static ImU32 Header                 = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.31f));
      inline static ImU32 HeaderHovered          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.80f));
      inline static ImU32 HeaderActive           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
      inline static ImU32 Separator              = Border;
      inline static ImU32 SeparatorHovered       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.10f, 0.40f, 0.75f, 0.78f));
      inline static ImU32 SeparatorActive        = ImGui::ColorConvertFloat4ToU32(ImVec4(0.10f, 0.40f, 0.75f, 1.00f));
      inline static ImU32 ResizeGrip             = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.20f));
      inline static ImU32 ResizeGripHovered      = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.67f));
      inline static ImU32 ResizeGripActive       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.95f));
      inline static ImU32 Tab                    = Header;
      inline static ImU32 TabHovered             = HeaderHovered;
      inline static ImU32 TabActive              = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 0.31f));
      inline static ImU32 TabUnfocused           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.04f, 0.04f, 0.04f, 1.00f));
      inline static ImU32 TabUnfocusedActive     = ImGui::ColorConvertFloat4ToU32(ImVec4{0.28f, 0.2805f, 0.281f, 1.0f});
      inline static ImU32 DockingPreview         = ImGui::ColorConvertFloat4ToU32(ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
      inline static ImU32 DockingEmptyBg         = ImGui::ColorConvertFloat4ToU32(ImVec4(0.20f, 0.20f, 0.20f, 1.00f));
      inline static ImU32 PlotLines              = ImGui::ColorConvertFloat4ToU32(ImVec4(0.61f, 0.61f, 0.61f, 1.00f));
      inline static ImU32 PlotLinesHovered       = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 0.43f, 0.35f, 1.00f));
      inline static ImU32 PlotHistogram          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.90f, 0.70f, 0.00f, 1.00f));
      inline static ImU32 PlotHistogramHovered   = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 0.60f, 0.00f, 1.00f));
      inline static ImU32 TableHeaderBg          = ImGui::ColorConvertFloat4ToU32(ImVec4(0.19f, 0.19f, 0.20f, 1.00f));
      inline static ImU32 TableBorderStrong      = ImGui::ColorConvertFloat4ToU32(ImVec4(0.31f, 0.31f, 0.35f, 1.00f));
      inline static ImU32 TableBorderLight       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.23f, 0.23f, 0.25f, 1.00f));
      inline static ImU32 TableRowBg             = ImGui::ColorConvertFloat4ToU32(ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
      inline static ImU32 TableRowBgAlt          = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 0.06f));
      inline static ImU32 DragDropTarget         = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 0.00f, 0.90f));
      inline static ImU32 NavHighlight           = ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.00f));
      inline static ImU32 NavWindowingHighlight  = ImGui::ColorConvertFloat4ToU32(ImVec4(1.00f, 1.00f, 1.00f, 0.70f));
      inline static ImU32 NavWindowingDimBg      = ImGui::ColorConvertFloat4ToU32(ImVec4(0.80f, 0.80f, 0.80f, 0.20f));
      inline static ImU32 ModalWindowDimBg       = ImGui::ColorConvertFloat4ToU32(ImVec4(0.80f, 0.80f, 0.80f, 0.35f));
      
      // Call this API After changing theme color variables
      static void Update();
    };
  };
} // namespace IKan::UI
