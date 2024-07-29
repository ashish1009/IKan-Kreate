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
} // namespace Kreator::UI
