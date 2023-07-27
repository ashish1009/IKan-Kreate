//
//  IKMemory.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "IKMemory.hpp"

namespace IKan
{
  static IKan::AllocationStats s_GlobalStats;
  
  bool Allocator::s_inInit = false;
  AllocatorData* Allocator::s_data = nullptr;
  
  void Allocator::Initialize()
  {
    if (s_data)
    {
      return;
    }
    
    s_inInit = true;
    AllocatorData* data = (AllocatorData*)Allocator::AllocateRaw(sizeof(AllocatorData));
    new(data) AllocatorData();
    s_data = data;
    s_inInit = false;
  }
  
  void Allocator::Shutdown()
  {
    delete s_data;
  }
  
  void* Allocator::AllocateRaw(size_t size)
  {
    return malloc(size);
  }
  
  void* Allocator::Allocate(size_t size)
  {
    if (s_inInit)
    {
      return AllocateRaw(size);
    }
    
    if (!s_data)
    {
      Initialize();
    }
    
    void* memory = malloc(size);
    
    {
      std::scoped_lock<std::mutex> lock(s_data->m_Mutex);
      Allocation& alloc = s_data->m_AllocationMap[memory];
      alloc.memory = memory;
      alloc.size = size;
      
      s_GlobalStats.totalAllocated += size;
    }
    
    return memory;
  }
  
  void* Allocator::Allocate(size_t size, const char* desc)
  {
    if (!s_data)
    {
      Initialize();
    }
    
    void* memory = malloc(size);
    
    Allocation& alloc = s_data->m_AllocationMap[memory];
    alloc.memory = memory;
    alloc.size = size;
    alloc.category = desc;
    
    s_GlobalStats.totalAllocated += size;
    if (desc)
    {
      s_data->m_allocationStatsMap[desc].totalAllocated += size;
    }

    return memory;
  }
  
  void* Allocator::Allocate(size_t size, const char* file, int line)
  {
    if (!s_data)
    {
      Initialize();
    }
    
    void* memory = malloc(size);
    
    Allocation& alloc = s_data->m_AllocationMap[memory];
    alloc.memory = memory;
    alloc.size = size;
    alloc.category = file;
    
    s_GlobalStats.totalAllocated += size;
    s_data->m_allocationStatsMap[file].totalAllocated += size;
    
    return memory;
  }
  
  void Allocator::Free(void* memory)
  {
    if (memory == nullptr)
    {
      return;
    }
    
    if (s_data->m_AllocationMap.find(memory) != s_data->m_AllocationMap.end())
    {
      std::scoped_lock<std::mutex> lock(s_data->m_Mutex);
      const Allocation& alloc = s_data->m_AllocationMap.at(memory);
      s_GlobalStats.totalFreed += alloc.size;
      if (alloc.category)
      {
        s_data->m_allocationStatsMap[alloc.category].totalFreed += alloc.size;
      }
      
      s_data->m_AllocationMap.erase(memory);
    }
    else
    {
      IK_LOG_WARN("Memory", "Memory block {0} not present in alloc map", memory);
    }
    
    free(memory);
  }
  
  namespace Memory
  {
    const AllocationStats& GetAllocationStats()
    {
      return s_GlobalStats;
    }
  }
} // namespace IKan

#ifdef IK_TRACK_MEMORY

void* operator new(size_t size)
{
  return IKan::Allocator::Allocate(size);
}

void* operator new[](size_t size)
{
  return IKan::Allocator::Allocate(size);
}

void* operator new(size_t size, const char* desc)
{
  return IKan::Allocator::Allocate(size, desc);
}

void* operator new[](size_t size, const char* desc)
{
  return IKan::Allocator::Allocate(size, desc);
}

void* operator new(size_t size, const char* file, int line)
{
  return IKan::Allocator::Allocate(size, file, line);
}

void* operator new[](size_t size, const char* file, int line)
{
  return IKan::Allocator::Allocate(size, file, line);
}

void operator delete(void* memory) noexcept
{
  return IKan::Allocator::Free(memory);
}

void operator delete(void* memory, const char* desc)
{
  return IKan::Allocator::Free(memory);
}

void operator delete(void* memory, const char* file, int line)
{
  return IKan::Allocator::Free(memory);
}

void operator delete[](void* memory) noexcept
{
  return IKan::Allocator::Free(memory);
}

void operator delete[](void* memory, const char* desc)
{
  return IKan::Allocator::Free(memory);
}

void operator delete[](void* memory, const char* file, int line)
{
  return IKan::Allocator::Free(memory);
}

#endif
