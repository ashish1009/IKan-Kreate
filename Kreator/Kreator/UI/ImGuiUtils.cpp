//
//  ImGuiUtils.cpp
//  Kreator
//
//  Created by Ashish . on 17/07/24.
//

#include "ImGuiUtils.hpp"

namespace Kreator::UI
{
  static int32_t s_UIContextID = 0;
  static uint32_t s_counter = 0;
  static char s_bufferID[16] = "##";
  static char s_labeledBufferID[1024];

  // Begin End Util API ---------------------------------------------------------------------------------------------
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
  void EndDisabled()
  {
    if (GImGui->DisabledStackSize > 0)
      ImGui::EndDisabled();
  }
  bool BeginPopup(const char* strID, ImGuiWindowFlags flags)
  {
    bool opened = false;
    if (ImGui::BeginPopup(strID, flags))
    {
      opened = true;
      // Fill background wiht nice gradient
      const float padding = ImGui::GetStyle().WindowBorderSize;
      const ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), -padding, -padding);
      ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
      
      const ImColor col1 = ImGui::GetStyleColorVec4(ImGuiCol_PopupBg);// Colours::Theme::backgroundPopup;
      const ImColor col2 = UI::ColorWithMultipliedValue(col1, 0.8f);
      
      ImGui::GetWindowDrawList()->AddRectFilledMultiColor(windowRect.Min, windowRect.Max, col1, col1, col2, col2);
      ImGui::GetWindowDrawList()->AddRect(windowRect.Min, windowRect.Max, UI::ColorWithMultipliedValue(col1, 1.1f));
      ImGui::PopClipRect();
      
      // Popped in EndPopup()
      ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(0, 0, 0, 80));
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
    }
    
    return opened;
  }
  
  void EndPopup()
  {
    ImGui::PopStyleVar(); // WindowPadding;
    ImGui::PopStyleColor(); // HeaderHovered;
    ImGui::EndPopup();
  }
  bool BeginTreeNode(const char* name, bool defaultOpen)
  {
    ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
    if (defaultOpen)
    {
      treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
    }
    return ImGui::TreeNodeEx(name, treeNodeFlags);
  }
  
  void EndTreeNode()
  {
    ImGui::TreePop();
  }
  
  // Cursor ----------------------------------------------------------------------------------------------------------
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
  void ShiftCursor(const ImVec2& val)
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
  void Separator()
  {
    ImGui::Separator();
  }
  void NewLine()
  {
    ImGui::NewLine();
  }
  
  // Wrappers --------------------------------------------------------------------------------------------------------
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

  ImTextureID GetTextureID(Ref<IKan::Texture> texture)
  {
    return (ImTextureID)INT2VOIDP(texture->GetRendererID());
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
  const char* GenerateLabelID(const std::string_view& label)
  {
    *fmt::format_to_n(s_labeledBufferID, std::size(s_labeledBufferID), "{}##{}", label, s_counter++).out = 0;
    return s_labeledBufferID;
  }
  
  bool IsItemHovered()
  {
    return ImGui::IsItemHovered();
  }
  bool IsItemDisabled()
  {
    return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
  }
  void SetTooltip(const std::string_view& text)
  {
    if (IsItemHovered())
    {
      UI::ScopedColor textCol(ImGuiCol_Text, UI::Color::TextBrighter);
      ImGui::SetTooltip(text.data());
    }
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
  
  // Draw APIs -------------------------------------------------------------------------------------------------------
  void DrawFilledRect(const ImU32& color, float height, float widthFactor, const glm::vec2& offset, float rounding)
  {
    // Draw the title Bar rectangle ---------------------------------------------------
    const ImVec2 titlebarMin =
    {
      ImGui::GetCursorScreenPos().x + offset.x,
      ImGui::GetCursorScreenPos().y + offset.y
    };
    const ImVec2 titlebarMax =
    {
      ImGui::GetCursorScreenPos().x + offset.x + (ImGui::GetWindowWidth() * widthFactor),
      ImGui::GetCursorScreenPos().y + offset.y + height
    };
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(titlebarMin, titlebarMax, color, rounding);
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
  // Buttons ---------------------------------------------------------------------------------------------------------
  bool ImageButton(const Ref<IKan::Image>& texture, const ImVec2& size, const ImVec4& bgCol, const ImVec4& tintCol,
                   const ImVec2& uv0, const ImVec2& uv1)
  {
    if (!texture)
    {
      return false;
    }
    ImGuiID id = texture->GetRendererID();
    return ImGui::ImageButtonEx(id, INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, bgCol, tintCol);
  }
  bool DrawButton(std::string_view title, FontType fontType, const ImU32& textColor, const ImU32& buttonColor,
                  float rounding, const glm::vec2& size)
  {
    ImGui::PushID(title.data());
    
    UI::ScopedStyle roundingStyle(ImGuiStyleVar_FrameRounding, rounding);
    UI::ScopedColor textColorStyle(ImGuiCol_Text, textColor);
    UI::ScopedFont headerStyle(UI::Font::Get(fontType));
    
    glm::vec3 color = Color::Vec3FromU32(buttonColor);
    ImVec4 tintNormal = {color.r, color.g, color.b, 0.8};
    ImVec4 tintHovered = {color.r, color.g, color.b, 0.9};
    ImVec4 tintPressed = {color.r, color.g, color.b, 1.0};
    
    UI::ScopedColor button(ImGuiCol_Button, tintNormal);
    UI::ScopedColor buttonhovered(ImGuiCol_ButtonHovered, tintHovered);
    UI::ScopedColor buttonavtive(ImGuiCol_ButtonActive, tintPressed);
    
    bool clicked = ImGui::Button(title.data(), {size.x, size.y});
    
    ImGui::PopID();
    return clicked;
  }
  void DrawButtonImage(const Ref<IKan::Image>& imageNormal, const Ref<IKan::Image>& imageHovered,
                       const Ref<IKan::Image>& imagePressed, const ImVec2& rectMin, const ImVec2& rectMax,
                       const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed)
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

  void DrawButtonImage(const Ref<IKan::Image>& image, const ImRect& rectangle, const ImU32& tintNormal, const ImU32& tintHovered,
                       const ImU32& tintPressed)
  {
    DrawButtonImage(image, image, image, rectangle.Min, rectangle.Max, tintNormal, tintHovered, tintPressed);
  }

  void DrawButtonImage(const Ref<IKan::Image>& image, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed)
  {
    DrawButtonImage(image, image, image, ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), tintNormal, tintHovered, tintPressed);
  }

  bool DrawButtonImage(std::string_view title, const Ref<IKan::Image>& texture, const glm::vec2& size, const glm::vec2& offset,
                       bool highlightHover, const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed)
  {
    UI::ShiftCursorX(offset.x);
    UI::ShiftCursorY(offset.y);
    const bool clicked = ImGui::InvisibleButton(title.data(), ImVec2(size.x, size.y), ImGuiButtonFlags_AllowItemOverlap);
    
    // Modify the icon size if hovered
    glm::vec2 iconSizeModified = size;
    glm::vec2 hoveredLargeIconOffset = {0.0f, 0.0f};
    if (IsItemHovered() and highlightHover)
    {
      iconSizeModified.x += 20.0f;
      iconSizeModified.y += 20.0f;
      
      hoveredLargeIconOffset = {5.0f, 5.0f};
    }
    
    const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x - hoveredLargeIconOffset.x,
      ImGui::GetItemRectMin().y - hoveredLargeIconOffset.y
    };
    const ImVec2 logoRectMax =
    {
      logoRectStart.x + iconSizeModified.x,
      logoRectStart.y + iconSizeModified.y
    };
    
    DrawButtonImage(texture, ImRect{logoRectStart, logoRectMax}, tintNormal, tintHovered, tintPressed);
    return clicked;
  }
  
  bool DrawImageTextButton(const std::string& buttonHelper, Ref<IKan::Image> icon, const glm::vec2& iconSize, const glm::vec2& offset,
                           const ImU32& tintNormal, const ImU32& tintHovered, const ImU32& tintPressed)
  {
    auto textSize = ImGui::CalcTextSize(buttonHelper.c_str()).x;;
    const float buttonWidth = textSize + iconSize.x + 80; // Some random number for margin
    
    UI::ShiftCursorX(offset.x);
    UI::ShiftCursorY(offset.y);
    if (ImGui::InvisibleButton(GenerateLabelID(buttonHelper), ImVec2(buttonWidth, iconSize.y), ImGuiButtonFlags_AllowItemOverlap))
    {
      return true;
    }
    
    // Modify the icon size if hovered
    glm::vec2 iconSizeModified = iconSize;
    glm::vec2 hoveredLargeIconOffset = {0.0f, 0.0f};
    if (IsItemHovered())
    {
      iconSizeModified.x += 10.0f;
      iconSizeModified.y += 10.0f;
      
      hoveredLargeIconOffset = {5.0f, 5.0f};
    }
    
    const ImVec2 logoRectStart =
    {
      ImGui::GetItemRectMin().x - hoveredLargeIconOffset.x,
      ImGui::GetItemRectMin().y - hoveredLargeIconOffset.y
    };
    const ImVec2 logoRectMax =
    {
      logoRectStart.x + iconSizeModified.x,
      logoRectStart.y + iconSizeModified.y
    };
    
    UI::DrawButtonImage(icon, ImRect{logoRectStart, logoRectMax}, tintNormal, tintHovered, tintPressed);
    
    ImGui::SameLine(offset.x + iconSizeModified.x + 10);
    UI::Text(UI::FontType::SemiHeader, buttonHelper, UI::AlignX::Left, {0.0f, iconSize.x / 4}, UI::Color::TextBrighter);
    return false;
  };
  
  // Shadow ----------------------------------------------------------------------------------------------------------
  void DrawShadowInnerImpl(const Ref<IKan::Image>& shadowImage, Position position, int radius, const ImVec2& rectMin, const ImVec2& rectMax, float alpha, float lengthStretch)
  {
    const float widthOffset = lengthStretch;
    const float alphaTop = alpha; //std::min(0.25f * alphMultiplier, 1.0f);
    const float alphaSides = alpha; //std::min(0.30f * alphMultiplier, 1.0f);
    const float alphaBottom = alpha; //std::min(0.60f * alphMultiplier, 1.0f);
    const auto p1 = ImVec2(rectMin.x + radius, rectMin.y + radius);
    const auto p2 = ImVec2(rectMax.x - radius, rectMax.y - radius);
    auto* drawList = ImGui::GetWindowDrawList();
    
    ImTextureID textureID = GetTextureID(shadowImage);
    
    switch (position)
    {
      case Position::Top:
        drawList->AddImage(textureID, { p1.x - widthOffset,  p1.y - radius }, { p2.x + widthOffset, p1.y }, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImColor(0.0f, 0.0f, 0.0f, alphaTop));
        break;
      case Position::Bottom:
        drawList->AddImage(textureID, { p1.x - widthOffset,  p2.y }, { p2.x + widthOffset, p2.y + radius }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), ImColor(0.0f, 0.0f, 0.0f, alphaBottom));
        break;
      case Position::Left:
        drawList->AddImageQuad(textureID, { p1.x - radius, p1.y - widthOffset }, { p1.x, p1.y - widthOffset }, { p1.x, p2.y + widthOffset }, { p1.x - radius, p2.y + widthOffset },
                               { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
        break;
      case Position::Right:
        drawList->AddImageQuad(textureID, { p2.x, p1.y - widthOffset }, { p2.x + radius, p1.y - widthOffset },
                               { p2.x + radius, p2.y + widthOffset }, { p2.x, p2.y + widthOffset },{ 0.0f, 1.0f },
                               { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, ImColor(0.0f, 0.0f, 0.0f, alphaSides));
        break;
      default:
        break;
    }
  }
  
  void DrawShadowInner(const Ref<IKan::Image>& shadowImage, Position position, int radius, const ImRect& rectangle, float alpha, float lengthStretch)
  {
    DrawShadowInnerImpl(shadowImage, position, radius, rectangle.Min, rectangle.Max, alpha, lengthStretch);
  }
  
  void DrawShadowInner(const Ref<IKan::Image>& shadowTexture, Position position, int32_t radius, float alpha)
  {
    // Draw side shadow
    ImRect windowRect = UI::RectExpanded(ImGui::GetCurrentWindow()->Rect(), 0.0f, 0.0f);
    ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
    UI::DrawShadowInner(shadowTexture, position, radius, windowRect, alpha, windowRect.GetHeight() / 4.0f);
    ImGui::PopClipRect();
  }
  
  // Texts -----------------------------------------------------------------------------------------------------------
  void Text(FontType type, std::string_view string, AlignX xAlign, const glm::vec2& offset, const ImU32& color)
  {
    UI::ScopedFont header(UI::Font::Get(type));
    UI::ScopedColor muted(ImGuiCol_Text, color);
    
    float xOffset = offset.x;
    switch (xAlign)
    {
      case AlignX::Center:
        xOffset += ImGui::GetColumnWidth() / 2 - ImGui::CalcTextSize(string.data()).x / 2;
        break;
      case AlignX::Right:
        xOffset += ImGui::GetColumnWidth() - ImGui::CalcTextSize(string.data()).x ;
        break;
      case AlignX::Left:
      default:
        break;
    }
    
    UI::SetCursor({ImGui::GetCursorPosX() + xOffset, ImGui::GetCursorPosY() + offset.y});
    ImGui::Text("%s", string.data());
  }

  // Image / Texture --------------------------------------------------------------------------------------------------
  void Image(const Ref<IKan::Texture>& texture, const ImVec2& size, const ImVec4& tintCol, const ImVec4& borderCol, const ImVec2& uv0, const ImVec2& uv1)
  {
    ImGui::Image(INT2VOIDP(texture->GetRendererID()), size, uv0, uv1, tintCol, borderCol);
  }
  void Image(Ref<IKan::Image> image, const glm::vec2& logoSize, AlignX xAlign, const glm::vec2& offset)
  {
    ImVec2 logoRectStart { ImGui::GetItemRectMin().x, ImGui::GetItemRectMin().y };
    
    // Shift the cursor
    logoRectStart.x += offset.x;
    logoRectStart.y += offset.y;

    // Update the rectangle start based on the x Position
    switch (xAlign)
    {
      case AlignX::Center:
        logoRectStart.x += (ImGui::GetColumnWidth() / 2 - logoSize.x / 2);
        break;
      case AlignX::Right:
        logoRectStart.x += (ImGui::GetColumnWidth() - logoSize.x);
        break;
      case AlignX::Left:
      default:
        break;
    }
    
    const ImVec2 logoRectMax
    {
      logoRectStart.x + logoSize.x,
      logoRectStart.y + logoSize.y
    };
    
    auto* drawList = ImGui::GetWindowDrawList();
    drawList->AddImage(UI::GetTextureID(image), logoRectStart, logoRectMax, {0, 1}, {1, 0});
  }
  
  // Imgui Customs ---------------------------------------------------------------------------------------------------
  bool TreeNode(const std::string& id, const std::string& label, ImGuiTreeNodeFlags flags, const Ref<IKan::Image>& icon)
  {
    // ImGui item height hack
    auto* currentWindow = ImGui::GetCurrentWindow();
    currentWindow->DC.CurrLineSize.y = 20.0f;
    currentWindow->DC.CurrLineTextBaseOffset = 3.0f;
    
    UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
    {
      return false;
    }
    
    bool clicked = UI::TreeNodeWithIcon(icon, window->GetID(id.c_str()), flags, label.c_str(), NULL);
    
    // Fixing slight overlap
    UI::ShiftCursorY(3.0f);
    return clicked;
  }
  
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end, const ImColor& iconTint/* = IM_COL32_WHITE*/)
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    ImGuiContext& g = *GImGui;
    ImGuiLastItemData& lastItem = g.LastItemData;
    const ImGuiStyle& style = g.Style;
    const bool display_frame = (flags & ImGuiTreeNodeFlags_Framed) != 0;
    const ImVec2 padding = (display_frame or (flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
    
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
    if (!display_frame and (flags & (ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth)) == 0)
      interact_bb.Max.x = frame_bb.Min.x + text_width + style.ItemSpacing.x * 2.0f;
    
    // Store a flag for the current depth to tell if we will allow closing this node when navigating one of its child.
    // For this purpose we essentially compare if g.NavIdIsAlive went from 0 to 1 between TreeNode() and TreePop().
    // This is currently only support 32 level deep and we are fine with (1 << Depth) overflowing into a zero.
    const bool is_leaf = (flags & ImGuiTreeNodeFlags_Leaf) != 0;
    bool is_open = ImGui::TreeNodeBehaviorIsOpen(id, flags);
    if (is_open and !g.NavIdIsAlive and (flags & ImGuiTreeNodeFlags_NavLeftJumpsBackHere) and !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
      window->DC.TreeJumpToParentOnPopMask |= (1 << window->DC.TreeDepth);
    
    bool item_add = ImGui::ItemAdd(interact_bb, id);
    lastItem.StatusFlags |= ImGuiItemStatusFlags_HasDisplayRect;
    lastItem.DisplayRect = frame_bb;
    
    if (!item_add)
    {
      if (is_open and !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
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
    const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 and g.IO.MousePos.x < arrow_hit_x2);
    if (window != g.HoveredWindow or !is_mouse_x_over_arrow)
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
      if (pressed and g.DragDropHoldJustPressedId != id)
      {
        if ((flags & (ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick)) == 0 or (g.NavActivateId == id))
          toggled = true;
        if (flags & ImGuiTreeNodeFlags_OpenOnArrow)
          toggled |= is_mouse_x_over_arrow and !g.NavDisableMouseHover; // Lightweight equivalent of IsMouseHoveringRect() since ButtonBehavior() already did the job
        if ((flags & ImGuiTreeNodeFlags_OpenOnDoubleClick) and g.IO.MouseDoubleClicked[0])
          toggled = true;
      }
      else if (pressed and g.DragDropHoldJustPressedId == id)
      {
        IM_ASSERT(button_flags & ImGuiButtonFlags_PressedOnDragDropHold);
        if (!is_open) // When using Drag and Drop "hold to open" we keep the node highlighted after opening, but never close it again.
          toggled = true;
      }
      
      if (g.NavId == id and g.NavMoveDir == ImGuiDir_Left and is_open)
      {
        toggled = true;
        ImGui::NavMoveRequestCancel();
      }
      if (g.NavId == id and g.NavMoveDir == ImGuiDir_Right and !is_open) // If there's something upcoming on the line we may want to give it the priority?
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
    
    const ImU32 arrow_col = selected ? UI::Color::BackgroundDark : UI::Color::TextMuted;
    
    ImGuiNavHighlightFlags nav_highlight_flags = ImGuiNavHighlightFlags_TypeThin;
    if (display_frame)
    {
      // Framed type
      const ImU32 bg_col = ImGui::GetColorU32((held and hovered) ? ImGuiCol_HeaderActive : (hovered and !selected and !held and !pressed and !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
      
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
        UI::Image(icon, { frame_height - pad * 2.0f, frame_height - pad * 2.0f }, iconTint, {0, 0, 0, 0}, ImVec2(0, 1), ImVec2(1, 0));
        
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
      if (hovered or selected)
      {
        //if (held and hovered) HZ_CORE_WARN("held and hovered");
        //if(hovered and !selected and !held and !pressed and !toggled) HZ_CORE_WARN("hovered and !selected and !held");
        //else if(!selected) HZ_CORE_WARN("ImGuiCol_Header");
        
        const ImU32 bg_col = ImGui::GetColorU32((held and hovered) ? ImGuiCol_HeaderActive : (hovered and !selected and !held and !pressed and !toggled) ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
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
        UI::Image(icon, { frame_height - pad * 2.0f, frame_height - pad * 2.0f }, iconTint, {0, 0, 0, 0}, ImVec2(0, 1), ImVec2(1, 0));
        
        // Restore itme data
        ImGui::SetLastItemData(itemId, itemFlags, itemStatusFlags, itemRect);
        
        text_pos.x += frame_height + 2.0f;
      }
      
      text_pos.y -= 1.0f;
      
      
      if (g.LogEnabled)
        ImGui::LogRenderedText(&text_pos, ">");
      ImGui::RenderText(text_pos, label, label_end, false);
    }
    
    if (is_open and !(flags & ImGuiTreeNodeFlags_NoTreePushOnOpen))
      ImGui::TreePushOverrideID(id);
    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | (is_leaf ? 0 : ImGuiItemStatusFlags_Openable) | (is_open ? ImGuiItemStatusFlags_Opened : 0));
    return is_open;
  }
  
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, const void* ptr_id, ImGuiTreeNodeFlags flags, const ImColor& iconTint, const char* fmt, ...)
  {
    va_list args;
    va_start(args, fmt);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    ImGuiContext& g = *GImGui;
    const char* label_end = g.TempBuffer.Data + ImFormatStringV(g.TempBuffer.Data, (size_t)g.TempBuffer.Size, fmt, args);
    
    bool is_open = TreeNodeWithIcon(icon, window->GetID(ptr_id), flags, g.TempBuffer.Data, label_end, iconTint);
    
    va_end(args);
    return is_open;
  }
  
  bool TreeNodeWithIcon(Ref<IKan::Image> icon, const char* label, ImGuiTreeNodeFlags flags, const ImColor& iconTint /*= IM_COL32_WHITE*/)
  {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
      return false;
    
    return TreeNodeWithIcon(icon, window->GetID(label), flags, label, NULL, iconTint);
  }
} //namespace Kreator::UI
