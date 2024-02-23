//
//  Buffer.cpp
//  IKan
//
//  Created by Ashish . on 23/02/24.
//

#include "Buffer.hpp"

namespace IKan
{
  Buffer::Buffer(std::byte* data, uint32_t size) : data(data), size(size)
  {
    
  }
  Buffer::Buffer(const Buffer& other)
  : size(other.size)
  {
    data = new std::byte[size];
    memcpy(data, other.data, size);
    s_allocatedNytes += size;
  }
  Buffer::Buffer(Buffer&& other)
  : data(other.data),  size(other.size)
  {
    other.size = 0;
    other.data = nullptr;
  }
  
  Buffer::~Buffer()
  {
    Clear();
  }
  
  void Buffer::Clear()
  {
    if (data)
    {
      Dealocate();
    }
    
    size = 0;
    data = nullptr;
  }
  
  void Buffer::ZeroInitialize()
  {
    if (data)
    {
      memset(data, 0, size);
    }
  }
  
  void Buffer::Write(void* new_data, uint32_t allocated_size, uint32_t offset)
  {
    RETURN_IF(allocated_size == 0);
    IK_ASSERT(offset + allocated_size <= size or offset > size, "Buffer overflow!");
    memcpy(data + offset, new_data, allocated_size);
  }
  void Buffer::Allocate(uint32_t allocated_size)
  {
    RETURN_IF(allocated_size == 0);
    
    Clear();
    size = allocated_size;
    data = new std::byte[size];
    
    s_allocatedNytes += size;
  }
  
  constexpr uint32_t Buffer::GetSize() const
  {
    return size;
  }
  
  Buffer::operator bool () const
  {
    return data;
  }
  
  std::byte& Buffer::operator[](uint32_t index)
  {
    return data[index];
  }
  
  std::byte Buffer::operator[](uint32_t index) const
  {
    return data[index];
  }
  
  Buffer& Buffer::operator =(const Buffer& other)
  {
    Clear();
    Allocate(other.size);
    Write((void*)other.data, other.size);
    return *this;
  }
  
  Buffer& Buffer::operator =(Buffer&& other)
  {
    Clear();
    
    size = other.size;
    data = other.data;
    
    other.size = 0;
    other.data = nullptr;
    
    return *this;
  }
  
  Buffer Buffer::Copy(void* data, uint32_t size)
  {
    Buffer buffer;
    
    if (size == 0)
    {
      return buffer;
    }
    
    buffer.Allocate(size);
    memcpy(buffer.data, data, size);
    return buffer;
  }
  
  uint32_t Buffer::GetTotalAllocations()
  {
    return s_allocatedNytes;
  }
  
  void Buffer::Dealocate()
  {
    s_allocatedNytes -= size;
    delete[] data;
  }
} // namespace IKan
