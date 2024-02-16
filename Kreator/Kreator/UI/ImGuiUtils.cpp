//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  static int32_t s_UIContextID{0};
  static uint32_t s_counter{0};
  static char s_bufferID[16] = "##";

  void PushID()
  {
    ImGui::PushID(s_UIContextID++);
    s_counter = 0;
  }  
  void PopID()
  {
    ImGui::PopID();
    s_UIContextID--;
  }
  const char* GenerateID()
  {
    std::string result {"##"};
    result += std::to_string(s_counter++);
    memcpy(s_bufferID, result.c_str(), 16);
    return s_bufferID;
  }
  ImTextureID GetTextureID(Ref<IKan::Texture> texture)
  {
    return (ImTextureID)INT2VOIDP(texture->GetRendererID());
  }

  void SetNextWindowAtCenter()
  {
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  }
  void SetNextWindowAtCenterWithSize(const ImVec2& size)
  {
    SetNextWindowAtCenter();
    SetNextWindowSize(size);
  }
  void SetNextWindowPos(const ImVec2& pos)
  {
    ImGui::SetNextWindowPos(pos);
  }
  void SetNextWindowSize(const ImVec2& size)
  {
    ImGui::SetNextWindowSize(size);
  }
  
  void BeginDisabled(bool disabled)
  {
    if (disabled)
    {
      ImGui::BeginDisabled(true);
    }
  }
  void EndDisabled()
  {
    if (GImGui->DisabledStackSize > 0)
    {
      ImGui::EndDisabled();
    }
  }

  void ShiftCursorX(float distance)
  {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
  }
  void ShiftCursorY(float distance)
  {
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
  }
  void ShiftCursor(float x, float y)
  {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
  }
  void ShiftCursor(ImVec2 val)
  {
    const ImVec2 cursor = ImGui::GetCursorPos();
    ImGui::SetCursorPos(ImVec2(cursor.x + val.x, cursor.y + val.y));
  }
  
  void SetCursorPos(float x, float y)
  {
    ImGui::SetCursorPos({x, y});
  }
  void SetCursorPos(const ImVec2& val)
  {
    ImGui::SetCursorPos(val);
  }
  void SetCursorPosX(float x)
  {
    ImGui::SetCursorPosX(x);
  }
  void SetCursorPosY(float y)
  {
    ImGui::SetCursorPosY(y);
  }
  void SameLine(float offsetFromStartX, float spacing)
  {
    ImGui::SameLine(offsetFromStartX, spacing);
  }
  void NewLine()
  {
    ImGui::NewLine();
  }

  // Rectangles API -------------------------------------------------------------------------------------------------
  ImRect GetItemRect()
  {
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
  
  ImRect RectExpanded(const ImRect& rect, float x, float y)
  {
    ImRect result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
  }
  
  ImRect RectOffset(const ImRect& rect, const ImVec2& xy)
  {
    return RectOffset(rect, xy.x, xy.y);
  }
  
  ImRect RectOffset(const ImRect& rect, float x, float y)
  {
    ImRect result = rect;
    result.Min.x += x;
    result.Min.y += y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
  }

  // Colors ----------------------------------------------------------------------------------------------------------
  ImU32 ColorWithValue(const ImColor& color, float value)
  {
    const ImVec4& colRow = color.Value;
    float hue, sat, val;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
    return ImColor::HSV(hue, sat, std::min(value, 1.0f));
  }
  
  ImU32 ColorWithSaturation(const ImColor& color, float saturation)
  {
    const ImVec4& colRow = color.Value;
    float hue, sat, val;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
    return ImColor::HSV(hue, std::min(saturation, 1.0f), val);
  }
  
  ImU32 ColorWithHue(const ImColor& color, float hue)
  {
    const ImVec4& colRow = color.Value;
    float h, s, v;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, h, s, v);
    return ImColor::HSV(std::min(hue, 1.0f), s, v);
  }
  
  ImU32 ColorWithMultipliedValue(const ImColor& color, float multiplier)
  {
    const ImVec4& colRow = color.Value;
    float hue, sat, val;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
    return ImColor::HSV(hue, sat, std::min(val * multiplier, 1.0f));
  }
  
  ImU32 ColorWithMultipliedSaturation(const ImColor& color, float multiplier)
  {
    const ImVec4& colRow = color.Value;
    float hue, sat, val;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
    return ImColor::HSV(hue, std::min(sat * multiplier, 1.0f), val);
  }
  
  ImU32 ColorWithMultipliedHue(const ImColor& color, float multiplier)
  {
    const ImVec4& colRow = color.Value;
    float hue, sat, val;
    ImGui::ColorConvertRGBtoHSV(colRow.x, colRow.y, colRow.z, hue, sat, val);
    return ImColor::HSV(std::min(hue * multiplier, 1.0f), sat, val);
  }

  // Image API -------------------------------------------------------------------------------------------------
  void Image(const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tintCol, const ImVec4& borderCol)
  {
    ImGui::Image(INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, tintCol, borderCol);
  }
  
  // Buttons ------------------------------------------------------------------------------------------------------
  bool ImageButton(const Ref<IKan::Texture>& image, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding,
                   const ImVec4& bg_col, const ImVec4& tint_col)
  {
    return ImageButton(nullptr, image, size, uv0, uv1, frame_padding, bg_col, tint_col);
  }
  
  bool ImageButton(const char* stringID, const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1,
                   int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
  {
    if (!texture)
    {
      return false;
    }
    ImGuiID id = texture->GetRendererID();
    return ImGui::ImageButtonEx(id, INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, bg_col, tint_col);
  }
  bool InvisibleButton(const ImVec2& size)
  {
    UI::ScopedColor noTintButton(ImGuiCol_Button, IM_COL32(11, 11, 11, 0));
    UI::ScopedColor noTintButtonHovered(ImGuiCol_ButtonHovered, IM_COL32(11, 11, 11, 0));
    UI::ScopedColor noTintButtonActive(ImGuiCol_ButtonActive, IM_COL32(11, 11, 11, 0));
    return ImGui::Button(GenerateID(), size);
  }
  
  bool DrawRoundButton(const char* title, glm::vec3 color, float r)
  {
    ImGui::PushID(title);
    UI::ScopedStyle rounding(ImGuiStyleVar_FrameRounding, r);
    
    ImVec4 tintNormal = {color.r, color.g, color.b, 0.4};
    ImVec4 tintHovered = {color.r, color.g, color.b, 0.3};
    ImVec4 tintPressed = {color.r, color.g, color.b, 0.5};
    
    UI::ScopedColor button(ImGuiCol_Button, tintNormal);
    UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, tintHovered);
    UI::ScopedColor buttonavtive(ImGuiCol_ButtonActive, tintPressed);
    
    bool clicked = ImGui::Button(title);
    
    ImGui::PopID();
    return clicked;
  }

  // Draw APIs -------------------------------------------------------------------------------------------------------
  void DrawButtonImage(const Ref<IKan::Texture>& imageNormal, const Ref<IKan::Texture>& imageHovered,
                       const Ref<IKan::Texture>& imagePressed, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed, const ImVec2& rectMin, const ImVec2& rectMax)
  {
    auto* drawList = ImGui::GetWindowDrawList();
    if (ImGui::IsItemActive())
    {
      drawList->AddImage(GetTextureID(imagePressed), rectMin, rectMax, ImVec2(0, 1), ImVec2(1, 0), tintPressed);
    }
    else if (ImGui::IsItemHovered())
    {
      drawList->AddImage(GetTextureID(imageHovered), rectMin, rectMax, ImVec2(0, 1), ImVec2(1, 0), tintHovered);
    }
    else
    {
      drawList->AddImage(GetTextureID(imageNormal), rectMin, rectMax, ImVec2(0, 1), ImVec2(1, 0), tintNormal);
    }
  }
  
  void DrawButtonImage(const Ref<IKan::Texture>& imageNormal, const Ref<IKan::Texture>& imageHovered,
                       const Ref<IKan::Texture>& imagePressed, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed, const ImRect& rectangle)
  {
    DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered,
                    tintPressed, rectangle.Min, rectangle.Max);
  }
  
  void DrawButtonImage(const Ref<IKan::Texture>& image, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed, const ImVec2& rectMin, const ImVec2& rectMax)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, rectMin, rectMax);
  };
  
  void DrawButtonImage(const Ref<IKan::Texture>& image, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed, const ImRect& rectangle)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed,
                    rectangle.Min, rectangle.Max);
  }
  
  void DrawButtonImage(const Ref<IKan::Texture>& imageNormal, const Ref<IKan::Texture>& imageHovered,
                       const Ref<IKan::Texture>& imagePressed, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed)
  {
    DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered,
                    tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
  
  void DrawButtonImage(const Ref<IKan::Texture>& image, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
  void DrawItemActivityOutline(float rounding, bool drawWhenInactive, const ImColor& colorWhenActive)
  {
    auto* drawList = ImGui::GetWindowDrawList();
    const ImRect rect = RectExpanded(GetItemRect(), 1.0f, 1.0f);
    if (ImGui::IsItemHovered() and !ImGui::IsItemActive())
    {
      drawList->AddRect(rect.Min, rect.Max, ImColor(60, 60, 60), rounding, 0, 1.5f);
    }
    if (ImGui::IsItemActive())
    {
      drawList->AddRect(rect.Min, rect.Max, colorWhenActive, rounding, 0, 1.0f);
    }
    else if (!ImGui::IsItemHovered() and drawWhenInactive)
    {
      drawList->AddRect(rect.Min, rect.Max, ImColor(50, 50, 50), rounding, 0, 1.0f);
    }
  }
  
  void DrawUnderline(bool fullWidth, float offsetX, float offsetY)
  {
    if (fullWidth)
    {
      if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
      {
        ImGui::PushColumnsBackground();
      }
      else if (ImGui::GetCurrentTable() != nullptr)
      {
        ImGui::TablePushBackgroundChannel();
      }
    }
    
    const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
    const ImVec2 cursor = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddLine(ImVec2(cursor.x + offsetX, cursor.y + offsetY), ImVec2(cursor.x + width, cursor.y + offsetY), UI::Color::BackgroundDark, 1.0f);
    
    if (fullWidth)
    {
      if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
      {
        ImGui::PopColumnsBackground();
      }
      else if (ImGui::GetCurrentTable() != nullptr)
      {
        ImGui::TablePopBackgroundChannel();
      }
    }
  }
  
  void DrawShadow(const Ref<IKan::Texture>& shadowImage, int radius, const ImVec2& rectMin, const ImVec2& rectMax, float alphMultiplier,
                  float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    const float widthOffset = lengthStretch;
    const float alphaTop = std::min(0.25f * alphMultiplier, 1.0f);
    const float alphaSides = std::min(0.30f * alphMultiplier, 1.0f);
    const float alphaBottom = std::min(0.60f * alphMultiplier, 1.0f);
    const auto p1 = rectMin;
    const auto p2 = rectMax;
    
    ImTextureID textureID = GetTextureID(shadowImage);
    
    auto* drawList = ImGui::GetWindowDrawList();
    if (drawLeft)
    {
      drawList->AddImage(textureID, { p1.x - widthOffset,  p1.y - radius }, { p2.x + widthOffset, p1.y },
                         ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(0.0f, 0.0f, 0.0f, alphaTop));
    }
    if (drawRight)
    {
      drawList->AddImage(textureID, { p1.x - widthOffset,  p2.y }, { p2.x + widthOffset, p2.y + radius },
                         ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(0.0f, 0.0f, 0.0f, alphaBottom));
    }
    if (drawTop)
    {
      drawList->AddImageQuad(textureID, { p1.x - radius, p1.y - widthOffset }, { p1.x, p1.y - widthOffset },
                             { p1.x, p2.y + widthOffset }, { p1.x - radius, p2.y + widthOffset }, { 0.0f, 0.0f },
                             { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
    }
    if (drawBottom)
    {
      drawList->AddImageQuad(textureID, { p2.x, p1.y - widthOffset }, { p2.x + radius, p1.y - widthOffset },
                             { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset }, { 0.0f, 1.0f },
                             { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
    }
  }
  
  void DrawShadow(const Ref<IKan::Texture>& shadowImage, int radius, const ImRect& rectangle, float alphMultiplier,
                  float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadow(shadowImage, radius, rectangle.Min, rectangle.Max, alphMultiplier, lengthStretch,
               drawLeft, drawRight, drawTop, drawBottom);
  }
  
  
  void DrawShadow(const Ref<IKan::Texture>& shadowImage, int radius, float alphMultiplier, float lengthStretch,
                  bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadow(shadowImage, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alphMultiplier,
               lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
  }
  
  void DrawShadowInner(const Ref<IKan::Texture>& shadowImage, int radius, const ImVec2& rectMin, const ImVec2& rectMax, float alpha,
                       float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    const float widthOffset = lengthStretch;
    const float alphaTop = alpha; //std::min(0.25f * alphMultiplier, 1.0f);
    const float alphaSides = alpha; //std::min(0.30f * alphMultiplier, 1.0f);
    const float alphaBottom = alpha; //std::min(0.60f * alphMultiplier, 1.0f);
    const auto p1 = ImVec2(rectMin.x + radius, rectMin.y + radius);
    const auto p2 = ImVec2(rectMax.x - radius, rectMax.y - radius);
    auto* drawList = ImGui::GetWindowDrawList();
    
    ImTextureID textureID = GetTextureID(shadowImage);
    
    if (drawTop)
    {
      drawList->AddImage(textureID, { p1.x - widthOffset,  p1.y - radius }, { p2.x + widthOffset, p1.y },
                         ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImColor(0.0f, 0.0f, 0.0f, alphaTop));
    }
    if (drawBottom)
    {
      drawList->AddImage(textureID, { p1.x - widthOffset,  p2.y }, { p2.x + widthOffset, p2.y + radius },
                         ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(0.0f, 0.0f, 0.0f, alphaBottom));
    }
    if (drawLeft)
    {
      drawList->AddImageQuad(textureID, { p1.x - radius, p1.y - widthOffset }, { p1.x, p1.y - widthOffset },
                             { p1.x, p2.y + widthOffset }, { p1.x - radius, p2.y + widthOffset }, { 0.0f, 1.0f },
                             { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
    }
    if (drawRight)
    {
      drawList->AddImageQuad(textureID, { p2.x, p1.y - widthOffset }, { p2.x + radius, p1.y - widthOffset },
                             { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset },{ 0.0f, 1.0f },
                             { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
    }
  }
  
  void DrawShadowInner(const Ref<IKan::Texture>& shadowImage, int radius, const ImRect& rectangle, float alpha,
                       float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadowInner(shadowImage, radius, rectangle.Min, rectangle.Max, alpha, lengthStretch, drawLeft,
                    drawRight, drawTop, drawBottom);
  }
  
  void DrawShadowInner(const Ref<IKan::Texture>& shadowImage, int radius, float alpha, float lengthStretch,
                       bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadowInner(shadowImage, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alpha,
                    lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
  }
  
  // Imgui Customs ---------------------------------------------------------------------------------------------------
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, const ImColor& iconTint/* = IM_COL32_WHITE*/)
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    ImGuiContext& g = *GImGui;
    ImGuiLastItemData& lastItem = g.LastItemData;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame || (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
    
    if (!label_end)
      label_end = ImGui::FindRenderedTextEnd(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, label_end, false);
    
    // We vertically grow up to current line height up the typical widget height.
    const float frame_height = ImMax(ImMin(window->DC.CurrLineSize.y, g.FontSize + style.FramePadding.y * 2), label_size.y + padding.y * 2);
    ImRect frame_bb;
    frame_bb.Min.x = (flags & ImGuiTreeNodeFlags_SpanFullWidth) ? window->WorkRect.Min.x : window->DC.CursorPos.x;
    frame_bb.Min.y = window->DC.CursorPos.y;
    frame_bb.Max.x = window->WorkRect.Max.x;
    frame_bb.Max.y = window->DC.CursorPos.y + frame_height;
    if (display_frame)
    {
      // Framed header expand a little outside the default padding, to the edge of InnerClipRect
      frame_bb.Min.x -= IM_FLOOR(window->WindowPadding.x * 0.5f - 1.0f);
      frame_bb.Max.x += IM_FLOOR(window->WindowPadding.x * 0.5f);
    }
    
    const float text_offset_x = g.FontSize + (display_frame ? padding.x * 3 : padding.x * 2);           // Collapser arrow width + Spacing
    const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
    const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
    ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
    ImGui::ItemSize(ImVec2(text_width, frame_height), padding.y);
    
    // For regular tree nodes, we arbitrary allow to click past 2 worth of ItemSpacing
    ImRect interact_bb = frame_bb;
    if (!display_frame && (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
      interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;
    
    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    bool is_open = ImGui::TreeNodeBehaviorIsOpen(id, flags);
    if (is_open && !g.NavIdIsAlive && (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
      window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);
    
    bool item_add = ImGui::ItemAdd(interact_bb, id);
    lastItem.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    lastItem.DisplayRect = frame_bb;
    
    if (!item_add)
    {
      if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
        ImGui::TreePushOverrideID(id);
      IMGUI_TEST_ENGINE_ITEM_INFO(lastItem.ID, label, lastItem.StatusFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
      return is_open;
    }
    
    ImGuiButtonFlags button_flags = ImGuiTreeNodeFlags_None;
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
      button_flags |= ImGuiButtonFlags_AllowItemOverlap;
    if (!is_leaf)
      button_flags |= ImGuiButtonFlags_PressedOnDragDropHold;
    
    // We allow clicking on the arrow section with keyboard modifiers held, in order to easily
    // allow browsing a tree while preserving selection with code implementing multi-selection patterns.
    // When clicking on the rest of the tree node we always disallow keyboard modifiers.
    const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
    const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);
    if (window != g.HoveredWindow || !is_mouse_x_over_arrow)
      button_flags |= ImGuiButtonFlags_NoKeyModifiers;
    
    // Open behaviors can be altered with the _OpenOnArrow and _OnOnDoubleClick flags.
    // Some alteration have subtle effects (e.g. toggle on MouseUp vs MouseDown events) due to requirements for multi-selection and drag and drop support.
    // - Single-click on label = Toggle on MouseUp (default, when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=0)
    // - Single-click on arrow = Toggle on MouseDown (when _OpenOnArrow=1)
    // - Double-click on label = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1)
    // - Double-click on arrow = Toggle on MouseDoubleClick (when _OpenOnDoubleClick=1 and _OpenOnArrow=0)
    // It is rather standard that arrow click react on Down rather than Up.
    // We set ImGuiButtonFlags_PressedOnClickRelease on OpenOnDoubleClick because we want the item to be active on the initial MouseDown in order for drag and drop to work.
    if (is_mouse_x_over_arrow)
      button_flags |= ImGuiButtonFlags_PressedOnClick;
    else if (flags & ImGuiTreeNodeFlags_OpenOnDoubleClick)
      button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    else
      button_flags |= ImGuiButtonFlags_PressedOnClickRelease;
    
    bool selected = (flags & ImGuiTreeNodeFlags_Selected) != 0;
    const bool was_selected = selected;
    
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(interact_bb, id, &hovered, &held, button_flags);
    bool toggled = false;
    if (!is_leaf)
    {
      if (pressed && g.DragDropHoldJustPressedId != id)
      {
        if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 || (g.NavActivateId == id))
          toggled = true;
        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
          toggled |= is_mouse_x_over_arrow && !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
        if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) && g.IO.MouseDoubleClicked[0])
          toggled = true;
      }
      else if (pressed && g.DragDropHoldJustPressedId == id)
      {
        IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
        if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
          toggled = true;
      }
      
      if (g.NavId == id && g.NavMoveDir == ImGuiDir_Left && is_open)
      {
        toggled = true;
        ImGui::NavMoveRequestCancel();
      }
      if (g.NavId == id && g.NavMoveDir == ImGuiDir_Right && !is_open) // If there's something upcoming on the line we may want to give it the priority?
      {
        toggled = true;
        ImGui::NavMoveRequestCancel();
      }
      
      if (toggled)
      {
        is_open = !is_open;
        window->DC.StateStorage->SetInt(id, is_open);
        lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledOpen;
      }
    }
    if (flags & ImGuiTreeNodeFlags_AllowItemOverlap)
      ImGui::SetItemAllowOverlap();
    
    // In this branch, TreeNodeBehavior() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
      lastItem.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;
    
    // Render
    
    const ImU32 arrow_col = selected ? UI::Color::BackgroundDark : UI::Color::Muted;
    
    ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
    if (display_frame)
    {
      // Framed type
      const ImU32 bg_col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !selected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
      
      ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, true, style.FrameRounding);
      ImGui::RenderNavHighlight(frame_bb, id, nav_highlight_flags);
      if (flags & ImGuiTreeNodeFlags_Bullet)
        ImGui::RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.60f, text_pos.y + g.FontSize * 0.5f), arrow_col);
      else if (!is_leaf)
        ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y), arrow_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 1.0f);
      else // Leaf without bullet, left-adjusted text
        text_pos.x -= text_offset_x;
      if (flags & ImGuiTreeNodeFlags_ClipLabelForTrailingButton)
        frame_bb.Max.x -= g.FontSize + style.FramePadding.x;
      
      //! Draw icon
      if (icon)
      {
        // Store item data
        auto itemId = lastItem.ID;
        auto itemFlags = lastItem.InFlags;
        auto itemStatusFlags = lastItem.StatusFlags;
        auto itemRect = lastItem.Rect;
        
        // Draw icon image which messes up last item data
        const float pad = 3.0f;
        const float arrowWidth = 20.0f + 1.0f;
        [[maybe_unused]] auto cursorPos = ImGui::GetCursorPos();
        UI::ShiftCursorY(-frame_height + pad);
        UI::ShiftCursorX(arrowWidth);
        UI::Image(icon, { frame_height - pad * 2.0f, frame_height - pad * 2.0f }, ImVec2(0, 1), ImVec2(1, 0),
                  iconTint /*selected ? colourDark : tintFloat*/);
        
        // Restore itme data
        ImGui::SetLastItemData(itemId, itemFlags, itemStatusFlags, itemRect);
        
        text_pos.x += frame_height + 2.0f;
      }
      
      text_pos.y -= 1.0f;
      
      
      
      if (g.LogEnabled)
      {
        // NB: '##' is normally used to hide text (as a library-wide feature), so we need to specify the text range to make sure the ## aren't stripped out here.
        const char log_prefix[] = "\n##";
        const char log_suffix[] = "##";
        ImGui::LogRenderedText(&text_pos, log_prefix, log_prefix + 3);
        ImGui::RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
        ImGui::LogRenderedText(&text_pos, log_suffix, log_suffix + 2);
      }
      else
      {
        ImGui::RenderTextClipped(text_pos, frame_bb.Max, label, label_end, &label_size);
      }
    }
    else
    {
      // Unframed typed for tree nodes
      if (hovered || selected)
      {
        //if (held && hovered) HZ_CORE_WARN("held && hovered");
        //if(hovered && !selected && !held && !pressed && !toggled) HZ_CORE_WARN("hovered && !selected && !held");
        //else if(!selected) HZ_CORE_WARN("ImGuiCol_Header");
        
        const ImU32 bg_col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : (hovered && !selected && !held && !pressed && !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, bg_col, false);
        ImGui::RenderNavHighlight(frame_bb, id, nav_highlight_flags);
      }
      if (flags & ImGuiTreeNodeFlags_Bullet)
        ImGui::RenderBullet(window->DrawList, ImVec2(text_pos.x - text_offset_x * 0.5f, text_pos.y + g.FontSize * 0.5f), arrow_col);
      else if (!is_leaf)
        ImGui::RenderArrow(window->DrawList, ImVec2(text_pos.x - text_offset_x + padding.x, text_pos.y + g.FontSize * 0.15f), arrow_col, is_open ? ImGuiDir_Down : ImGuiDir_Right, 0.70f);
      
      //! Draw icon
      if (icon)
      {
        // Store item data
        auto itemId = lastItem.ID;
        auto itemFlags = lastItem.InFlags;
        auto itemStatusFlags = lastItem.StatusFlags;
        auto itemRect = lastItem.Rect;
        
        // Draw icon image which messes up last item data
        const float pad = 3.0f;
        const float arrowWidth = 20.0f + 1.0f;
        [[maybe_unused]] auto cursorPos = ImGui::GetCursorPos();
        UI::ShiftCursorY(- frame_height + pad);
        UI::ShiftCursorX(arrowWidth);
        UI::Image(icon, { frame_height - pad * 2.0f, frame_height - pad * 2.0f }, ImVec2(0, 1), ImVec2(1, 0), iconTint /*selected ? colourDark : tintFloat*/);
        
        // Restore itme data
        ImGui::SetLastItemData(itemId, itemFlags, itemStatusFlags, itemRect);
        
        text_pos.x += frame_height + 2.0f;
      }
      
      text_pos.y -= 1.0f;
      
      
      if (g.LogEnabled)
        ImGui::LogRenderedText(&text_pos, ">");
      ImGui::RenderText(text_pos, label, label_end, false);
    }
    
    if (is_open && !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
      ImGui::TreePushOverrideID(id);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
    return is_open;
  }
  
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, const void* ptr_id, ImGuiTreeNodeFlags flags, const ImColor& iconTint, const char* fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer.Data + ImFormatStringV(g.TempBuffer.Data, g.TempBuffer.Size, fmt, args);
    
    bool is_open = TreeNodeWithIcon(icon, window->GetID(ptr_id), flags, g.TempBuffer.Data, label_end, iconTint);
    
    va_end(args);
    return is_open;
  }
  
  bool TreeNodeWithIcon(Ref<IKan::Texture> icon, const char* label, ImGuiTreeNodeFlags flags, const ImColor& iconTint /*= IM_COL32_WHITE*/)
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    return TreeNodeWithIcon(icon, window->GetID(label), flags, label, NULL, iconTint);
  }

} // namespace Kreator::UI
