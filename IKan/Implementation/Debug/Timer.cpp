//
//  Timer.cpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#include "Timer.hpp"

namespace IKan
{
  // Timer -----------------------------------------------------------------------------------------------------------
  Timer::Timer()
  {
    Reset();
  }
  void Timer::Reset()
  {
    m_time = std::chrono::high_resolution_clock::now();
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
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_time).count();
  }
  
  // Scoped Timer -----------------------------------------------------------------------------------------------------
  ScopedTimer::ScopedTimer(const char* functionName)
  : m_startTime(std::chrono::high_resolution_clock::now()), m_functionName(functionName)
  {
    
  }
  ScopedTimer::~ScopedTimer()
  {
    m_endTime = std::chrono::high_resolution_clock::now();
    m_duration = m_endTime - m_startTime;
    
    IK_PROFILE_INFO("{0} ms : {1}", m_duration.count() * 1000, m_functionName);
  }
  
  // Performance Profiler --------------------------------------------------------------------------------------------
  void PerformanceProfiler::SetPerFrameTiming(const char* name, float time)
  {
    // Store the function in map
    if (m_perFrameData.find(name) == m_perFrameData.end())
    {
      m_perFrameData[name] = 0.0f;
    }
    
    // Add the elapsed time if called for same function
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
  PerformanceProfiler& PerformanceProfiler::Get()
  {
    static PerformanceProfiler s_instance;
    return s_instance;
  }
  
  // Scope Performance Timer -----------------------------------------------------------------------------------------
  ScopePerfTimer::ScopePerfTimer(const char* name)
  : m_name(name)
  {
    
  }
  ScopePerfTimer::~ScopePerfTimer()
  {
    float time = static_cast<float>(m_timer.ElapsedMiliSeconds());
    PerformanceProfiler::Get().SetPerFrameTiming(m_name, time);
  }
} // namespace IKan
