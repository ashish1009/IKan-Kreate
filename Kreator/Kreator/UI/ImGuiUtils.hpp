//
//  ImGuiUtils.hpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

#include "UI/ImGuiTheme.hpp"

using namespace IKan;

namespace Kreator::UI
{
  /// This enum stores the ImGui item position in X Axis
  enum class AlignX : uint8_t
  {
    Left, Right, Center
  };

  // Cursor ----------------------------------------------------------------------------------------------------------
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
  /// - Parameter val: new shifted value
  void ShiftCursor(const ImVec2& val);
  /// This function shift the cursor position
  /// - Parameters:
  ///   - x: x value
  ///   - y: y value
  void SetCursorPos(float x, float y);
  /// This function shift the cursor position
  /// - Parameter val: New shift value
  void SetCursorPos(const ImVec2& val);
  /// This function shift the cursor position for x
  /// - Parameter distance: val x
  void SetCursorPosX(float x);
  /// This function shift the cursor position for y
  /// - Parameter distance: val y
  void SetCursorPosY(float y);
  /// This function shift the cursor position
  /// - Parameter val: New shift value
  void SetCursor(const ImVec2& val);
  /// This function create same line
  /// - Parameters:
  ///   - offsetFromStartX: offset from sstart point
  ///   - spacing: space after last item
  void SameLine(float offsetFromStartX = 0.0f, float spacing = -1.0f);
  /// This function create same line
  void NewLine();
  
  // Wrappers --------------------------------------------------------------------------------------------------------
  /// This function set the next imgui window at center
  void SetNextWindowAtCenter();
  /// This function set the next imgui window at center with size
  /// - Parameter size: size of next window
  void SetNextWindowAtCenterWithSize(const ImVec2& size);
  /// This function set the next imgui window position
  /// - Parameter pos: position of next window
  void SetNextWindowPos(const ImVec2& pos);
  /// This function set the next imgui window size
  /// - Parameter size: size of next window
  void SetNextWindowSize(const ImVec2& size);

  /// This function returns the Texutre Renderer ID for ImGui
  /// - Parameter texture: texture
  ImTextureID GetTextureID(Ref<Texture> texture);
  /// This function push unque id for ImGui
  void PushID();
  /// This function pop last used id for ImGui
  void PopID();
  /// This function generate unique ID for ImGui
  const char* GenerateID();
  /// This function generate Unique id for labeled widget
  /// - Parameter label: Widget lable
  const char* GenerateLabelID(const std::string_view& label);
  /// This function checkes is imgui item hovered
  bool IsItemHovered();
  /// This function checles is imgui item disabled
  bool IsItemDisabled();
  
  /// This function show the tooltip messge
  /// - Parameter text: message
  void SetTooltip(const std::string_view& text);
  
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
  
  // Buttons ---------------------------------------------------------------------------------------------------------
  /// This function draws the image button
  /// - Parameters:
  ///   - imageNormal: Normal Image icon
  ///   - imageHovered: Hovered Image icon
  ///   - imagePressed: Pressed Image Icon
  ///   - tintNormal: tint normal color
  ///   - tintHovered: tint hovered color
  ///   - tintPressed: tint pressed color
  ///   - rectMin: min rectangle position
  ///   - rectMax: max rectangle position
  /// - Note: Do not draw actual presaable button
  void DrawButtonImage(const Ref<IKan::Image>& imageNormal, const Ref<IKan::Image>& imageHovered,
                       const Ref<IKan::Image>& imagePressed, const ImVec2& rectMin, const ImVec2& rectMax,
                       const ImU32& tintNormal = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 0.8f),
                       const ImU32& tintHovered = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.2f),
                       const ImU32& tintPressed = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.0f));

  /// This function draws the image button
  /// - Parameters:
  ///   - image: Image
  ///   - tintNormal: tint normal color
  ///   - tintHovered: tint hovered color
  ///   - tintPressed: tint pressed color
  ///   - rectangle: image rectangle
  /// - Note: Do not draw actual presaable button
  void DrawButtonImage(const Ref<IKan::Image>& image, const ImRect& rectangle,
                       const ImU32& tintNormal = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 0.8f),
                       const ImU32& tintHovered = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.5f),
                       const ImU32& tintPressed = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.0f));
  /// This function Draws the button with image and title
  /// - Parameters:
  ///   - buttonHelper: button helper
  ///   - icon: Image
  ///   - iconSize: Button size
  ///   - offset: offset of button
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  bool DrawImageTextButton(const std::string& buttonHelper, Ref<IKan::Image> icon,
                           const glm::vec2& iconSize, const glm::vec2& offset = {0.0f, 0.0f},
                           const ImU32& tintNormal = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 0.8f),
                           const ImU32& tintHovered = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.5f),
                           const ImU32& tintPressed = UI::ColorWithMultipliedValue(UI::Color::TextBrighter, 1.0f));

  // Texts -----------------------------------------------------------------------------------------------------------
  /// This function renders the text
  /// - Parameters:
  ///   - type: Type of font
  ///   - string: string to be printed
  ///   - xAlign: alignment in x axis
  ///   - offset: offset from current cursor position
  void Text(FontType type, std::string_view string, AlignX xAlign, const glm::vec2& offset = {0, 0}, const ImU32& color = UI::Color::Text);

  // Image/ Texture ---------------------------------------------------------------------------------------------------
  /// This function renders Texture in current ImGui window
  /// - Parameters:
  ///   - texture: IKan texture reference pointer
  ///   - size: size of texture need to be rendered inside ImGui window (this size will be visible as texture)
  ///   - uv0: Top left texture coordinate
  ///   - uv1: Bottom Right texture coordinate
  ///   - tintCol: Color of texture image
  ///   - borderCol: Image Border color
  void Image(const Ref<Texture>& texture, const ImVec2& size, const ImVec4& tintCol = {1, 1, 1, 1}, const ImVec4& borderCol = {0, 0, 0, 0}, 
             const ImVec2& uv0 = {0, 1}, const ImVec2& uv1 = {1, 0});
  /// This function draws the image in rectangle
  /// - Parameters:
  ///   - image: Image reference pointer
  ///   - logoSize: logo size
  ///   - xAlign: poistion of logo based on cursor pointer in x Axis
  ///   - offset: offset from current cursor position
  /// - Note: Doesnt move the cursor
  void Image(Ref<IKan::Image> image, const glm::vec2& logoSize, AlignX xAlign, const glm::vec2& offset = {0, 0});
} // namespace Kreator::UI
