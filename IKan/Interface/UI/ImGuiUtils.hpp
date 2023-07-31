//
//  ImGuiUtils.hpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#pragma once

namespace IKan
{
  class Texture;
  class Image;
} // namespace IKan

namespace IKan::UI
{
  // Wrappers --------------------------------------------------------------------------------------------------------
  /// Add horizontal seprator
  void Separator();
  /// This function returns the Texutre Renderer ID for Imgui
  /// - Parameter texture: texture
  ImTextureID GetTextureID(Ref<Texture> texture);
  /// This function shift the cursor position for x
  /// - Parameter distance: distance x
  void ShiftCursorX(float distance);
  /// This function shift the cursor position for y
  /// - Parameter distance: distance y
  void ShiftCursorY(float distance);
  /// This function shift the cursor position
  /// - Parameters:
  ///   - x: x distance
  ///   - y: y distance
  void ShiftCursor(float x, float y);
  /// This function shift the cursor position
  /// - Parameters:
  ///   - x: x value
  ///   - y: y value
  void SetCursorPos(float x, float y);
  /// This function shift the cursor position
  /// - Parameter val: New shift value
  void SetCursorPos(ImVec2 val);
  /// This function shift the cursor position for x
  /// - Parameter distance: val x
  void SetCursorPosX(float x);
  /// This function shift the cursor position for y
  /// - Parameter distance: val y
  void SetCursorPosY(float y);
  /// This function shift the cursor position
  /// - Parameter val: New shift value
  void SetCursor(ImVec2 val);
  /// This function create same line
  /// - Parameters:
  ///   - offsetFromStartX: offset from sstart point
  ///   - spacing: space after last item
  void SameLine(float offsetFromStartX = 0.0f, float spacing = -1.0f);
  /// This function create same line
  void NewLine();
  /// This function checkes is imgui item hovered
  bool IsItemHovered();
  
  /// This function push unque id for Imgui
  void PushID();
  /// This function pop last used id for Imgui
  void PopID();
  /// This function generate unique ID for Imgui
  const char* GenerateID();
  /// This function generate Unique id for labeled widget
  /// - Parameter label: Widget lable
  const char* GenerateLabelID(std::string_view label);
  /// This function enable ior disable mouse
  /// - Parameter enable: flag
  void SetMouseEnabled(const bool enable);
  
  /// This function Check if navigated to current item, e.g. with arrow keys
  bool NavigatedTo();
  /// This function show the tooltip messge
  /// - Parameter text: message
  void SetTooltip(std::string_view text);

  // Begin End -------------------------------------------------------------------------------------------------------
  /// This function draws MenuBar which allows you to specify its rectangle
  bool BeginMenuBar(const ImRect& barRectangle);
  /// This function ends MenuBar
  void EndMenuBar();
  /// This function begin disable
  void BeginDisabled(bool disabled = true);
  /// This function Ends the disable
  void EndDisabled();

  // Rectangles API -------------------------------------------------------------------------------------------------
  /// This function returns the Imgui Item Rectangle vertex
  ImRect GetItemRect();
  /// This function expand the Imgui Retangle
  /// - Parameters:
  ///   - rect: rectangle
  ///   - x: expand x
  ///   - y: expand y
  ImRect RectExpanded(const ImRect& rect, float x, float y);
  /// This function Offset the rectangle
  /// - Parameters:
  ///   - rect: rectangle pos
  ///   - x: x offset
  ///   - y: y offset
  ImRect RectOffset(const ImRect& rect, float x, float y);
  /// This function Offset the rectangle
  /// - Parameters:
  ///   - rect: rectangle pos
  ///   - xy: offset
  ImRect RectOffset(const ImRect& rect, ImVec2 xy);

  // Colors ----------------------------------------------------------------------------------------------------------
  /// This function add the color to some value
  /// - Parameters:
  ///   - color: color
  ///   - value: value
  ImU32 ColorWithValue(const ImColor& color, float value);
  /// This function adds the color with saturation
  /// - Parameters:
  ///   - color: color
  ///   - saturation: saturation
  ImU32 ColorWithSaturation(const ImColor& color, float saturation);
  /// This function adds the color with Hue
  /// - Parameters:
  ///   - color: color
  ///   - hue: hue
  ImU32 ColorWithHue(const ImColor& color, float hue);
  /// This finction multiply the color with some value
  /// - Parameters:
  ///   - color: color
  ///   - multiplier: multiplier
  ImU32 ColorWithMultipliedValue(const ImColor& color, float multiplier);
  /// This finction multiply the color with some saturation
  /// - Parameters:
  ///   - color: color
  ///   - multiplier: multiplier
  ImU32 ColorWithMultipliedSaturation(const ImColor& color, float multiplier);
  /// This finction multiply the color with some hue
  /// - Parameters:
  ///   - color: color
  ///   - multiplier: multiplier
  ImU32 ColorWithMultipliedHue(const ImColor& color, float multiplier);
  
  // Draw APIs -------------------------------------------------------------------------------------------------------
  /// This function Draw the button Image
  /// - Parameters:
  ///   - imageNormal: Button Image texture
  ///   - imageHovered: Button Hovered Image texture
  ///   - imagePressed: Button pressed Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectMin: rect Min
  ///   - rectMax: rext Max
  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered, const Ref<Image>& imagePressed,
                       ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax);
  /// This function Draw the button Image
  /// - Parameters:
  ///   - imageNormal: Button Image texture
  ///   - imageHovered: Button Hovered Image texture
  ///   - imagePressed: Button pressed Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectangle: rectangle vertex
  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered, const Ref<Image>& imagePressed,
                       ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rectangle);
  /// This function Draw the button Image
  /// - Parameters:
  ///   - image: Button Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectMin: rect Min
  ///   - rectMax: rext Max
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax);
  /// This function Draw the button Image
  /// - Parameters:
  ///   - imageNormal: Button Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectangle: rectangle vertex
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed, ImRect rectangle);
  /// This function Draw the button Image
  /// - Parameters:
  ///   - imageNormal: Button Image texture
  ///   - imageHovered: Button Hovered Image texture
  ///   - imagePressed: Button pressed Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered, const Ref<Image>& imagePressed,
                       ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed);
  /// This function Draw the button Image
  /// - Parameters:
  ///   - image: Button Image texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed);
  
  /// This function draw the Border as rectangle
  /// - Parameters:
  ///   - rectMin: min bound
  ///   - rectMax: max Bound
  ///   - borderColor: border color
  void DrawBorder(ImVec2 rectMin, ImVec2 rectMax, const ImVec4& border_color, float thickness = 1.0f,
                  float offsetX = 0.0f, float offsetY = 0.0f);
  /// This function draw the Border as rectangle
  /// - Parameter borderColor: border color
  void DrawBorder(const ImVec4& borderColor, float thickness = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);
  /// This function draw the Border as rectangle
  void DrawBorder(float thickness = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);
  /// This function draw the Border as rectangle
  /// - Parameters:
  ///   - rectMin: min bound
  ///   - rectMax: max Bound
  void DrawBorder(ImVec2 rectMin, ImVec2 rectMax, float thickness = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f);
  /// This function draw the Border as rectangle
  /// - Parameter rect: rect vertex
  void DrawBorder(ImRect rect, float thickness = 1.0f, float rounding = 0.0f, float offsetX = 0.0f, float offsetY = 0.0f);
  

} //  namespace IKan::UI
