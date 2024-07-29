//
//  ImGuiInputBuffer.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator::UI
{
  /// This class stores the GUI Input buffer
  template <uint32_t SIZE> class InputBuffer
  {
  public:
    InputBuffer(const std::string& title)
    : m_title(title)
    {
      m_buffer = new char[SIZE];
    }
    
    ~InputBuffer()
    {
      delete m_buffer;
    }
    
    bool TextInput(bool readOnly = false, float width = -1, std::string_view hint = "", ImGuiInputTextFlags flag = 0)
    {
      bool modified = false;
      if (readOnly)
      {
        flag |= ImGuiInputTextFlags_ReadOnly;
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::TextMuted);
      }
      else
      {
        ImGui::PushStyleColor(ImGuiCol_Text, UI::Color::TextBrighter);
      }
      ImGui::SetNextItemWidth(width);
      if (hint != "")
      {
        modified = ImGui::InputTextWithHint(UI::GenerateLabelID(m_title), hint.data(), m_buffer, SIZE, flag);
      }
      else
      {
        modified = ImGui::InputTextWithHint(UI::GenerateLabelID(m_title), m_title.c_str(), m_buffer, SIZE, flag);
      }
      
      ImGui::PopStyleColor();
      return modified;
    }
    
    void Memset(char data)
    {
      memset(m_buffer, data, SIZE);
    }
    
    void MemCpy(const char* data, uint32_t offset, size_t size)
    {
      memccpy(m_buffer, data, 0, size);
    }
    
    void StrCpy(const std::string& data)
    {
      strcpy(m_buffer, data.c_str());
    }
    
    char* Data()
    {
      return m_buffer;
    }
    
    operator std::string()
    {
      return static_cast<std::string>(m_buffer);
    }
    
    constexpr uint32_t Size() const
    {
      return SIZE;
    }
    
    bool Empty() const
    {
      return (std::string)m_buffer == "";
    }
    
  private:
    char* m_buffer {nullptr};
    std::string m_title;
  };
  
} // namespace Kreator::UI
