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
  
  // Begin End -------------------------------------------------------------------------------------------------------
  /// This function draws MenuBar which allows you to specify its rectangle
  bool BeginMenuBar(const ImRect& barRectangle);
  /// This function ends MenuBar
  void EndMenuBar();

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

} //  namespace IKan::UI
