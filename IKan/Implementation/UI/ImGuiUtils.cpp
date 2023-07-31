//
//  ImGuiUtils.cpp
//  IKan
//
//  Created by Ashish . on 31/07/23.
//

#include "ImGuiUtils.hpp"
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

} // namespace IKan::UI
