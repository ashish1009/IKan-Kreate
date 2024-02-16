//
//  UI_InputBuffer.hpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#pragma once

namespace IKan
{
  template <uint32_t SIZE>
  class GUI_InputBuffer
  {
  public:
    GUI_InputBuffer()
    {
      m_buffer = new char[SIZE];
    }
    
    ~GUI_InputBuffer()
    {
      delete m_buffer;
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
    
  private:
    char* m_buffer {nullptr};
  };
} // namespace IKan
