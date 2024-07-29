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

  // Buttons ---------------------------------------------------------------------------------------------------------
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
} //namespace Kreator::UI
