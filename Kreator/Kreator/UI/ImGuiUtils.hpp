//
//  ImGuiUtils.hpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#pragma once

using namespace IKan;

namespace Kreator::UI
{
  /// This enum stores the ImGui item position in X Axis
  enum class AllignX : uint8_t
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
  ///   - xAllign: poistion of logo based on cursor pointer  in x Axis
  ///   - offset: offset from current cursor position
  /// - Note: Doesnt move the cursor
  void Image(Ref<IKan::Image> image, const glm::vec2& logoSize, AllignX xAllign, const glm::vec2& offset = {0, 0});
} // namespace Kreator::UI
