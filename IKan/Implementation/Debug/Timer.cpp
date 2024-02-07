//
//  Timer.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "Timer.hpp"

namespace IKan
{
  // Timer ------------------------------------------------------------------------------------------------------------
  Timer::Timer()
  {
    Reset();
  }
  
  void Timer::Reset()
  {
    m_startTime = std::chrono::high_resolution_clock::now();
  }
  
  double Timer::Elapsed() const
  {
    return ElapsedMiliSeconds() * double(0.001f);
  }
  
  double Timer::ElapsedMiliSeconds() const
  {
    return ElapsedMicroSeconds() * double(0.001f);
  }
  
  double Timer::ElapsedMicroSeconds() const
  {
    return ElapsedNanoSeconds() * double(0.001f);
  }
  
  double Timer::ElapsedNanoSeconds() const
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count();
  }
  
  // Scoped Timer -----------------------------------------------------------------------------------------------------
  ScopedTimer::ScopedTimer(const char* functionName)
  : m_functionName(functionName)
  {
    m_startTime = std::chrono::high_resolution_clock::now();
  }
  
  ScopedTimer::~ScopedTimer()
  {
    m_endTime = std::chrono::high_resolution_clock::now();;
    m_duration = m_endTime - m_startTime;
        
    IK_PROFILE_INFO("{0} : {1} ms", m_functionName, m_duration.count() * 1000);
  }
  
  // Performance Profiler -------------------------------------------------------------------------------------------
  void PerformanceProfiler::SetPerFrameTiming(const char* name, float time)
  {
    if (m_perFrameData.find(name) == m_perFrameData.end())
    {
      m_perFrameData[name] = 0.0f;
    }
    
    m_perFrameData[name] += time;
  }
  
  void PerformanceProfiler::Clear()
  {
    m_perFrameData.clear();
  }
  
  const std::unordered_map<const char*, float>& PerformanceProfiler::GetPerFrameData() const
  {
    return m_perFrameData;
  }
  
  PerformanceProfiler* PerformanceProfiler::Get()
  {
    if (!s_instance)
    {
      s_instance = new PerformanceProfiler();
    }
    return s_instance;
  }
  
  void PerformanceProfiler::Destroy()
  {
    delete s_instance;
  }
  
  // Scope Performance Timer ----------------------------------------------------------------------------------------
  ScopePerfTimer::ScopePerfTimer(const char* name, PerformanceProfiler* profiler)
  : m_name(name), m_profiler(profiler)
  {
    
  }
  
  ScopePerfTimer::~ScopePerfTimer()
  {
    float time = m_timer.ElapsedMiliSeconds();
    m_profiler->SetPerFrameTiming(m_name, time);
  }
} // namespace IKan
