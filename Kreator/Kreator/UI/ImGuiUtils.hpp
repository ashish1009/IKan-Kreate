//
//  ImGuiUtils.hpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#pragma once

using namespace IKan;

namespace Kreator::UI
{
  /// This function push unque id for ImGui
  void PushID();
  /// This function pop last used id for ImGui
  void PopID();
  /// This function generate unique ID for ImGui
  const char* GenerateID();
  /// This function returns the Texutre Renderer ID for ImGui
  /// - Parameter texture: texture
  ImTextureID GetTextureID(Ref<Texture> texture);

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
  
  /// This function begin disable
  void BeginDisabled(bool disabled = true);
  /// This function Ends the disable
  void EndDisabled();

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
  void SetCursorPos(const ImVec2& val);
  /// This function shift the cursor position for x
  /// - Parameter distance: val x
  void SetCursorPosX(float x);
  /// This function shift the cursor position for y
  /// - Parameter distance: val y
  void SetCursorPosY(float y);
  /// This function create same line
  /// - Parameters:
  ///   - offsetFromStartX: offset from sstart point
  ///   - spacing: space after last item
  void SameLine(float offsetFromStartX = 0.0f, float spacing = -1.0f);
  /// This function create same line
  void NewLine();

} // namespace Kreator::UI
