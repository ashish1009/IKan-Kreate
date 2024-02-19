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
  /// This function checkes is imgui item hovered
  bool IsItemHovered();
  bool IsItemDisabled();
  
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
  
  /// This function Draw invisible button
  /// - Parameter size: size of button
  bool InvisibleButton(const ImVec2& size);
  /// This function creates the rounding button
  /// - Parameters:
  ///   - title: Title
  ///   - color: Background color
  ///   - rounding: Rounding of button
  bool DrawRoundButton(const char* title, const glm::vec3& color, float rounding);
  /// This function creates the rounding button
  /// - Parameters:
  ///   - title: Title
  ///   - color: Background color
  ///   - rounding: Rounding of button
  bool DrawRoundButton(const char* title, const ImU32& color, float rounding);

  /// This function Check if navigated to current item, e.g. with arrow keys
  bool NavigatedTo();
  
  /// This function renders the Tree node
  /// - Parameters:
  ///   - id: Item ID
  ///   - label: Item Lable
  ///   - flags: Item Flags
  ///   - icon: Item Icon Texture
  bool TreeNode(const std::string& id, const std::string& label, ImGuiTreeNodeFlags flags = 0, const Ref<Texture>& icon = nullptr);
  /// This function show the tooltip messge
  /// - Parameter text: message
  void SetTooltip(const std::string_view& text);
  
  // Begin End -------------------------------------------------------------------------------------------------------
  /// This function draws MenuBar which allows you to specify its rectangle
  bool BeginMenuBar(const ImRect& barRectangle);
  /// This function ends MenuBar
  void EndMenuBar();
  /// This function begin disable
  void BeginDisabled(bool disabled = true);
  /// This function Ends the disable
  void EndDisabled();
  /// This function begins the popup
  /// - Parameter strID: string ID
  bool BeginPopup(const char* strID, ImGuiWindowFlags flags = 0);
  /// This function ends the popup
  void EndPopup();
  /// This function begins the tree node
  /// - Parameter name: ndoe name
  bool BeginTreeNode(const char* name, bool defaultOpen = true);
  /// This function ends the tree node
  void EndTreeNode();
  
  // Rectangles API -------------------------------------------------------------------------------------------------
  /// This function returns the ImGui Item Rectangle vertex
  ImRect GetItemRect();
  /// This function expand the ImGui Retangle
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
  ImRect RectOffset(const ImRect& rect, const ImVec2& xy);
  
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
  
  // Buttons -----------------------------------------------------------------------------------------------------------
  /// This function renders the Texture button
  /// - Parameters:
  ///   - Texture: Texture Texture
  ///   - size: Button Size
  bool TextureButton(const Ref<Texture>& Texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1),
                     const ImVec2& uv1 = ImVec2(1, 0), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                     const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
  /// This function renders the Texture button
  /// - Parameters:
  ///   - stringID: string ID
  ///   - Texture: Texture Texture
  ///   - size: Buttoren Size
  bool TextureButton(const char* stringID, const Ref<Texture>& Texture, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1),
                     const ImVec2& uv1 = ImVec2(1, 0), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                     const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
  
  // Image -----------------------------------------------------------------------------------------------------------
  /// This function renders Texture in current ImGui window
  /// - Parameters:
  ///   - texture; Texture reference
  ///   - size; size of Texture need to be rendered inside ImGui window (this size will be visible as texture)
  ///   - uv0; Left
  ///   - uv1; Right
  void Image(const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec2& uv0 = {0, 1}, const ImVec2& uv1 = {1, 0}, const ImVec4& tintCol = {1, 1, 1, 1}, const ImVec4& borderCol = {0, 0, 0, 0});
  /// This function renders the image button
  /// - Parameters:
  ///   - image: Image Texture
  ///   - size: Button Size
  bool ImageButton(const Ref<Texture>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1),
                   const ImVec2& uv1 = ImVec2(1, 0), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                   const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
  /// This function renders the image button
  /// - Parameters:
  ///   - stringID: string ID
  ///   - image: Image Texture
  ///   - size: Buttoren Size
  bool ImageButton(const char* stringID, const Ref<Texture>& image, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1),
                   const ImVec2& uv1 = ImVec2(1, 0), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
                   const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
  
  
  // Draw APIs -------------------------------------------------------------------------------------------------------
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - TextureNormal: Button Texture texture
  ///   - TextureHovered: Button Hovered Texture texture
  ///   - TexturePressed: Button pressed Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectMin: rect Min
  ///   - rectMax: rext Max
  void DrawButtonImage(const Ref<IKan::Texture>& TextureNormal, const Ref<IKan::Texture>& TextureHovered, const Ref<IKan::Texture>& TexturePressed,
                       const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed, const ImVec2& rectMin, const ImVec2& rectMax);
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - TextureNormal: Button Texture texture
  ///   - TextureHovered: Button Hovered Texture texture
  ///   - TexturePressed: Button pressed Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectangle: rectangle vertex
  void DrawButtonImage(const Ref<IKan::Texture>& TextureNormal, const Ref<IKan::Texture>& TextureHovered, const Ref<IKan::Texture>& TexturePressed,
                       const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed, const ImRect& rectangle);
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - Texture: Button Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectMin: rect Min
  ///   - rectMax: rext Max
  void DrawButtonImage(const Ref<IKan::Texture>& Texture, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed, const ImVec2& rectMin, const ImVec2& rectMax);
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - TextureNormal: Button Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  ///   - rectangle: rectangle vertex
  void DrawButtonImage(const Ref<IKan::Texture>& Texture, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed, const ImRect& rectangle);
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - TextureNormal: Button Texture texture
  ///   - TextureHovered: Button Hovered Texture texture
  ///   - TexturePressed: Button pressed Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  void DrawButtonImage(const Ref<IKan::Texture>& TextureNormal, const Ref<IKan::Texture>& TextureHovered, const Ref<IKan::Texture>& TexturePressed,
                       const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed);
  /// This function Draw the button Texture
  /// - Parameters:
  ///   - Texture: Button Texture texture
  ///   - tintNormal: tint normal
  ///   - tintHovered: tint hovered
  ///   - tintPressed: tint pressed
  void DrawButtonImage(const Ref<IKan::Texture>& Texture, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed);
  /// This function draw the Activity outline
  /// - Parameters:
  ///   - rounding: Outline rounding
  ///   - drawWhenInactive: Flag to draw when inactive
  ///   - colourWhenActive: Color when active
  void DrawItemActivityOutline(float rounding = 0.0f, bool drawWhenInactive = false, const ImColor& colourWhenActive = ImColor(80, 80, 80));
  
  /// This function renders the line in Ingui
  void DrawUnderline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f);
  
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  ///   - rectMin: rect min
  ///   - rectMax: rect Max
  void DrawShadow(const Ref<IKan::Texture>& shadowTexture, int radius, const ImVec2& rectMin, const ImVec2& rectMax,
                  float alphMultiplier = 1.0f, float lengthStretch = 10.0f, bool drawLeft = true, bool drawRight = true,
                  bool drawTop = true, bool drawBottom = true);
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  ///   - rectangle: rectangle Data
  void DrawShadow(const Ref<IKan::Texture>& shadowTexture, int radius, const ImRect& rectangle, float alphMultiplier = 1.0f, float lengthStretch = 10.0f,
                  bool drawLeft = true, bool drawRight = true, bool drawTop = true, bool drawBottom = true);
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  void DrawShadow(const Ref<IKan::Texture>& shadow_Texture, int radius, float alphMultiplier = 1.0f, float lengthStretch = 10.0f,
                  bool drawLeft = true, bool drawRight = true, bool drawTop = true, bool drawBottom = true);
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  ///   - rectMin: rect min
  ///   - rectMax: rect Max
  void DrawShadowInner(const Ref<IKan::Texture>& shadowTexture, int radius, const ImVec2& rectMin, const ImVec2& rectMax, float alpha = 1.0f,
                       float lengthStretch = 10.0f, bool drawLeft = true, bool drawRight = true, bool drawTop = true,
                       bool drawBottom = true);
  
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  ///   - rectangle: rectangle Data
  void DrawShadowInner(const Ref<IKan::Texture>& shadowTexture, int radius, const ImRect& rectangle, float alpha = 1.0f, float lengthStretch = 10.0f,
                       bool drawLeft = true, bool drawRight = true, bool drawTop = true, bool drawBottom = true);
  /// This function render the Shadow Texture in ImGui
  /// - Parameters:
  ///   - shadowTexture: shadow Texture
  ///   - radius: radius of shadow
  void DrawShadowInner(const Ref<IKan::Texture>& shadowTexture, int radius, float alpha = 1.0f, float lengthStretch = 10.0f,
                       bool drawLeft = true, bool drawRight = true, bool drawTop = true, bool drawBottom = true);
  
  // Imgui Customs ---------------------------------------------------------------------------------------------------
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, const ImColor& iconTint = IM_COL32_WHITE);
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, const void* ptr_id, ImGuiTreeNodeFlags flags, const ImColor& iconTint, const char* fmt, ...);
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, const char* label, ImGuiTreeNodeFlags flags, const ImColor& iconTint = IM_COL32_WHITE);
  
} // namespace Kreator::UI
