//
//  UI_Buffer.hpp
//  IKan
//
//  Created by Ashish . on 31/08/23.
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
      m_buffer = iknew char[SIZE];
    }
    
    ~GUI_InputBuffer()
    {
      ikdelete m_buffer;
    }
    
    void Memset(char data)
    {
      memset(m_buffer, data, SIZE);
    }
    
    void MemCpy(const char* data, uint32_t offset, size_t size)
    {
      memccpy(m_buffer, data, 0, size);
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
    char* m_buffer = nullptr;
  };
} // namespace IKan
