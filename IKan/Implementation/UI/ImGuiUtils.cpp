//
//  ImGuiUtils.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiUtils.hpp"
#include "UI/ImGuiScoped.hpp"
#include "UI/ImGuiTheme.hpp"
#include "Renderer/Graphics/Texture.hpp"

namespace IKan::UI
{
  static int32_t s_UIContextID = 0;
  static uint32_t s_counter = 0;
  static char s_bufferID[16] = "##";
  static char s_labeledBufferID[1024];

  // Wrappers --------------------------------------------------------------------------------------------------------
  void Separator()
  {
    ImGui::Separator();
  }
  ImTextureID GetTextureID(Ref<Texture> texture)
  {
    return (ImTextureID)INT2VOIDP(texture->GetRendererID());
  }
  void SetCursor(const ImVec2& pos)
  {
    ImGui::SetCursorPos(pos);
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
  void SetCursorPos(ImVec2 val)
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
  
  bool IsItemHovered()
  {
    return ImGui::IsItemHovered();
  }

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
    std::string result = "##";
    result += std::to_string(s_counter++);
    memcpy(s_bufferID, result.c_str(), 16);
    return s_bufferID;
  }
  
  const char* GenerateLabelID(std::string_view label)
  {
    *fmt::format_to_n(s_labeledBufferID, std::size(s_labeledBufferID), "{}##{}", label, s_counter++).out = 0;
    return s_labeledBufferID;
  }
  
  void SetMouseEnabled(const bool enable)
  {
    if (enable)
    {
      ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
    else
    {
      ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    }
  }
  
  bool NavigatedTo()
  {
    ImGuiContext& g = *GImGui;
    return g.NavJustMovedToId == g.LastItemData.ID;
  }
  
  void SetTooltip(std::string_view text)
  {
    if (IsItemHovered())
    {
      UI::ScopedColor textCol(ImGuiCol_Text, UI::Theme::Color::TextBrighter);
      ImGui::SetTooltip(text.data());
    }
  }

  // Begin End -------------------------------------------------------------------------------------------------------
  bool BeginMenuBar(const ImRect& barRectangle)
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
      return false;
    }
    
    IM_ASSERT(!window->DC.MenuBarAppending);
    ImGui::BeginGroup(); // Backup position on layer 0
    ImGui::PushID("##menubar");
    
    const ImVec2 padding = window->WindowPadding;
    
    // We don't clip with current window clipping rectangle as it is already set to the area below.
    // However we clip with window full rect. We remove 1 worth of rounding to Max.x to that text
    // in long menus and small windows don't tend to display over the lower-right rounded area, which
    // looks particularly glitchy.
    ImRect barRect = UI::RectOffset(barRectangle, 0.0f, padding.y);
    ImRect clipRect(IM_ROUND(ImMax(window->Pos.x, barRect.Min.x + window->WindowBorderSize + window->Pos.x - 10.0f)),
                    IM_ROUND(barRect.Min.y + window->WindowBorderSize + window->Pos.y),
                    IM_ROUND(ImMax(barRect.Min.x + window->Pos.x, barRect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize))),
                    IM_ROUND(barRect.Max.y + window->Pos.y));
    
    clipRect.ClipWith(window->OuterRectClipped);
    ImGui::PushClipRect(clipRect.Min, clipRect.Max, false);
    
    // We overwrite CursorMaxPos because BeginGroup sets it to CursorPos (essentially the Emit Item
    // hack in EndMenuBar() would need something analogous here, maybe a BeginGroupEx() with flags).
    window->DC.CursorPos = window->DC.CursorMaxPos = ImVec2(barRect.Min.x + window->Pos.x, barRect.Min.y + window->Pos.y);
    window->DC.LayoutType = ImGuiLayoutType_Horizontal;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;
    window->DC.MenuBarAppending = true;
    ImGui::AlignTextToFramePadding();
    
    return true;
  }
  
  void EndMenuBar()
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
      return;
    }
    
    ImGuiContext& g = *GImGui;
    
    // Nav: When a move request within one of our child menu failed, capture the request to
    // navigate among our siblings.
    if (ImGui::NavMoveRequestButNoResultYet() and
        (g.NavMoveDir == ImGuiDir_Left or g.NavMoveDir == ImGuiDir_Right) and
        (g.NavWindow->Flags & ImGuiWindowFlags_ChildMenu))
    {
      // Try to find out if the request is for one of our child menu
      ImGuiWindow* navEarliestChild = g.NavWindow;
      while (navEarliestChild->ParentWindow and (navEarliestChild->ParentWindow->Flags & ImGuiWindowFlags_ChildMenu))
      {
        navEarliestChild = navEarliestChild->ParentWindow;
      }
      
      if (navEarliestChild->ParentWindow == window and
          navEarliestChild->DC.ParentLayoutType == ImGuiLayoutType_Horizontal and
          (g.NavMoveFlags & ImGuiNavMoveFlags_Forwarded) == 0)
      {
        // To do so we claim focus back, restore NavId and then process the movement request
        // for yet another frame. This involve a one-frame delay which isn't very problematic
        // in this situation. We could remove it by scoring in advance for multiple
        // window (probably not worth bothering)
        const ImGuiNavLayer layer = ImGuiNavLayer_Menu;
        IM_ASSERT(window->DC.NavLayersActiveMaskNext & (1 << layer)); // Sanity check
        
        ImGui::FocusWindow(window);
        ImGui::SetNavID(window->NavLastIds[layer], layer, 0, window->NavRectRel[layer]);
        
        g.NavDisableHighlight = true;
        g.NavDisableMouseHover = g.NavMousePosDirty = true;
        ImGui::NavMoveRequestForward(g.NavMoveDir, g.NavMoveClipDir, g.NavMoveFlags, g.NavMoveScrollFlags); // Repeat
      }
    }
    
    // Static Analysis false positive "warning C6011: Dereferencing NULL pointer 'window'"
    // IM_ASSERT(window->Flags & ImGuiWindowFlags_MenuBar);
    IM_MSVC_WARNING_SUPPRESS(6011);
    
    IM_ASSERT(window->DC.MenuBarAppending);
    ImGui::PopClipRect();
    ImGui::PopID();
    
    // Save horizontal position so next append can reuse it. This is kinda equivalent to a per-layer CursorPos.
    window->DC.MenuBarOffset.x = window->DC.CursorPos.x - window->Pos.x;
    g.GroupStack.back().EmitItem = false;
    
    ImGui::EndGroup(); // Restore position on layer 0
    window->DC.LayoutType = ImGuiLayoutType_Vertical;
    window->DC.NavLayerCurrent = ImGuiNavLayer_Main;
    window->DC.MenuBarAppending = false;
  }
  
  void BeginDisabled(bool disabled)
  {
    if (disabled)
      ImGui::BeginDisabled(true);
  }
  
  bool IsItemDisabled()
  {
    return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
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
  
  ImRect RectOffset(const ImRect& rect, ImVec2 xy)
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
  
  // Draw APIs -------------------------------------------------------------------------------------------------------
  bool DrawRoundButton(const char* title, glm::vec3 color, float rounding)
  {
    ImGui::PushID(title);
    
    ImVec4 tintNormal = {color.r, color.g, color.b, 0.4};
    ImVec4 tintHovered = {color.r, color.g, color.b, 0.3};
    ImVec4 tintPressed = {color.r, color.g, color.b, 0.5};
    
    UI::ScopedColor button(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    UI::ScopedColor buttonavtive(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
    
    ImVec2 origCursonPos = ImGui::GetCursorScreenPos();
    if (ImGui::Button(title))
    {
      ImGui::PopID();
      return true;
    }

    // Round Bar -------------------------------------------------------------------------
    const ImVec2 roundBarMin = origCursonPos;
    const ImVec2 roundBarMax =
    {
      roundBarMin.x + ImGui::CalcTextSize(title).x + 20,
      roundBarMin.y + ImGui::GetFontSize() * 2
    };
    auto* drawList = ImGui::GetWindowDrawList();
    if (ImGui::IsItemActive())
    {
      drawList->AddRectFilled(roundBarMin, roundBarMax, ImGui::ColorConvertFloat4ToU32(tintPressed), rounding);
    }
    else if (ImGui::IsItemHovered())
    {
      drawList->AddRectFilled(roundBarMin, roundBarMax, ImGui::ColorConvertFloat4ToU32(tintHovered), rounding);
    }
    else
    {
      drawList->AddRectFilled(roundBarMin, roundBarMax, ImGui::ColorConvertFloat4ToU32(tintNormal), rounding);
    }

    ImGui::PopID();
    return false;
  }

  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered,
                       const Ref<Image>& imagePressed, ImU32 tintNormal, ImU32 tintHovered,
                       ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax)
  {
    auto* drawList = ImGui::GetWindowDrawList();
    if (ImGui::IsItemActive())
    {
      drawList->AddImage(GetTextureID(imagePressed), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintPressed);
    }
    else if (ImGui::IsItemHovered())
    {
      drawList->AddImage(GetTextureID(imageHovered), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintHovered);
    }
    else
    {
      drawList->AddImage(GetTextureID(imageNormal), rectMin, rectMax, ImVec2(0, 0), ImVec2(1, 1), tintNormal);
    }
  }
  
  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered,
                       const Ref<Image>& imagePressed, ImU32 tintNormal, ImU32 tintHovered,
                       ImU32 tintPressed, ImRect rectangle)
  {
    DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered,
                    tintPressed, rectangle.Min, rectangle.Max);
  }
  
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered,
                       ImU32 tintPressed, ImVec2 rectMin, ImVec2 rectMax)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, rectMin, rectMax);
  };
  
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered,
                       ImU32 tintPressed, ImRect rectangle)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed,
                    rectangle.Min, rectangle.Max);
  }
  
  void DrawButtonImage(const Ref<Image>& imageNormal, const Ref<Image>& imageHovered,
                       const Ref<Image>& imagePressed, ImU32 tintNormal, ImU32 tintHovered,
                       ImU32 tintPressed)
  {
    DrawButtonImage(imageNormal, imageHovered, imagePressed, tintNormal, tintHovered,
                    tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
  
  void DrawButtonImage(const Ref<Image>& image, ImU32 tintNormal, ImU32 tintHovered, ImU32 tintPressed)
  {
    DrawButtonImage(image, image, image, tintNormal, tintHovered, tintPressed, ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
  }
  
  void DrawBorder(const ImVec4& borderColor, float thickness, float offsetX, float offsetY)
  {
    DrawBorder(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), borderColor, thickness, offsetX, offsetY);
  }
  void DrawBorder(float thickness, float offsetX, float offsetY)
  {
    DrawBorder(ImGui::GetStyleColorVec4(ImGuiCol_Border), thickness, offsetX, offsetY);
  }
  void DrawBorder(ImVec2 rectMin, ImVec2 rectMax, float thickness, float offsetX, float offsetY)
  {
    DrawBorder(rectMin, rectMax, ImGui::GetStyleColorVec4(ImGuiCol_Border), thickness, offsetX, offsetY);
  }
  void DrawBorder(ImVec2 rectMin, ImVec2 rectMax, const ImVec4& borderColor, float thickness, float offsetX, float offsetY)
  {
    auto min = rectMin;
    min.x -= thickness;
    min.y -= thickness;
    min.x += offsetX;
    min.y += offsetY;
    auto max = rectMax;
    max.x += thickness;
    max.y += thickness;
    max.x += offsetX;
    max.y += offsetY;
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(borderColor), 0.0f, 0, thickness);
  };
  void DrawBorder(ImRect rect, float thickness, float rounding, float offsetX, float offsetY)
  {
    auto min = rect.Min;
    min.x -= thickness;
    min.y -= thickness;
    min.x += offsetX;
    min.y += offsetY;
    auto max = rect.Max;
    max.x += thickness;
    max.y += thickness;
    max.x += offsetX;
    max.y += offsetY;
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRect(min, max, ImGui::ColorConvertFloat4ToU32(ImGui::GetStyleColorVec4(ImGuiCol_Border)), rounding, 0, thickness);
  }
  
  void DrawShadow(const Ref<Texture>& shadowImage, int radius, ImVec2 rectMin, ImVec2 rectMax, float alphMultiplier,
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
  
  void DrawShadow(const Ref<Texture>& shadowImage, int radius, ImRect rectangle, float alphMultiplier,
                  float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadow(shadowImage, radius, rectangle.Min, rectangle.Max, alphMultiplier, lengthStretch,
               drawLeft, drawRight, drawTop, drawBottom);
  }
  
  
  void DrawShadow(const Ref<Texture>& shadowImage, int radius, float alphMultiplier, float lengthStretch,
                  bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadow(shadowImage, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alphMultiplier,
               lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
  }
  
  void DrawShadowInner(const Ref<Texture>& shadowImage, int radius, ImVec2 rectMin, ImVec2 rectMax, float alpha,
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
                             { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset },{ 0.0f, 0.0f },
                             { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
    }
  }
  
  void DrawShadowInner(const Ref<Texture>& shadowImage, int radius, ImRect rectangle, float alpha,
                       float lengthStretch, bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadowInner(shadowImage, radius, rectangle.Min, rectangle.Max, alpha, lengthStretch, drawLeft,
                    drawRight, drawTop, drawBottom);
  }
  
  void DrawShadowInner(const Ref<Texture>& shadowImage, int radius, float alpha, float lengthStretch,
                       bool drawLeft, bool drawRight, bool drawTop, bool drawBottom)
  {
    DrawShadowInner(shadowImage, radius, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), alpha,
                    lengthStretch, drawLeft, drawRight, drawTop, drawBottom);
  }
  
} // namespace IKan::UI
