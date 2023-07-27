//
//  IKMemory.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
  struct AllocationStats
  {
    size_t totalAllocated = 0;
    size_t totalFreed = 0;
  };
  
  struct Allocation
  {
    void* memory = 0;
    size_t size = 0;
    const char* category = 0;
  };
  
  namespace Memory
  {
    const AllocationStats& GetAllocationStats();
  } // namespace Memory
  
  template <class T>
  struct Mallocator
  {
    typedef T value_type;
    
    Mallocator() = default;
    template <class U> constexpr Mallocator(const Mallocator <U>&) noexcept {}
    
    T* allocate(std::size_t n)
    {
#undef max
      if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
        throw std::bad_array_new_length();
      
      if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
        return p;
      }
      
      throw std::bad_alloc();
    }
    
    void deallocate(T* p, std::size_t n) noexcept {
      std::free(p);
    }
  };
  
  struct AllocatorData
  {
    using MapAlloc = Mallocator<std::pair<const void* const, Allocation>>;
    using StatsMapAlloc = Mallocator<std::pair<const char* const, AllocationStats>>;
    
    using AllocationStatsMap = std::map<const char*, AllocationStats, std::less<const char*>, StatsMapAlloc>;
    
    std::map<const void*, Allocation, std::less<const void*>, MapAlloc> m_AllocationMap;
    AllocationStatsMap m_allocationStatsMap;
    
    std::mutex m_Mutex, m_StatsMutex;
  };
  
  class Allocator
  {
  public:
    static void Initialize();
    static void Shutdown();
    static void* AllocateRaw(size_t size);
    static void* Allocate(size_t size);
    static void* Allocate(size_t size, const char* desc);
    static void* Allocate(size_t size, const char* file, int line);
    static void Free(void* memory);
    
    static const AllocatorData::AllocationStatsMap& GetAllocationStats() { return s_data->m_allocationStatsMap; }
    
  private:
    static AllocatorData* s_data;
    static bool s_inInit;
  };
  
} // namespace IKan

#ifdef IK_TRACK_MEMORY

void* operator new(size_t size);
void* operator new[](size_t size);
void* operator new(size_t size, const char* desc);
void* operator new[](size_t size, const char* desc);
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);

void operator delete(void* memory) noexcept;
void operator delete(void* memory, const char* desc);
void operator delete(void* memory, const char* file, int line);
void operator delete[](void* memory) noexcept;
void operator delete[](void* memory, const char* desc);
void operator delete[](void* memory, const char* file, int line);

#define iknew new(__FILE__, __LINE__)
#define ikdelete delete

#else

#define iknew new
#define ikdelete delete

#endif
