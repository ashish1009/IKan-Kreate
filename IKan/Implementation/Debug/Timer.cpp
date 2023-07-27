//
//  Timer.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Timer.hpp"

namespace IKan
{
  // Timer ----------------------------------------------------------------------------------------------------
  Timer::Timer()
  {
    Reset();
  }
  
  void Timer::Reset()
  {
    m_startTime = std::chrono::high_resolution_clock::now();
  }
  
  float Timer::Elapsed() const
  {
    return ElapsedMiliSeconds() * 0.001f;
  }
  
  float Timer::ElapsedMiliSeconds() const
  {
    return ElapsedMicroSeconds() * 0.001f;
  }
  
  float Timer::ElapsedMicroSeconds() const
  {
    return ElapsedNanoSeconds() * 0.001f;
  }
  
  float Timer::ElapsedNanoSeconds() const
  {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_startTime).count();
  }
  // Scoped Timer ----------------------------------------------------------------------------------------------------
  ScopedTimer::ScopedTimer(const char* functionName)
  : m_functionName(functionName)
  {
    m_startTime = std::chrono::high_resolution_clock::now();
  }
  
  ScopedTimer::~ScopedTimer()
  {
    m_endTime = std::chrono::high_resolution_clock::now();;
    m_duration = m_endTime - m_startTime;

    std::string timeString = std::to_string(m_duration.count() * 1000);
    timeString += " ms";
    std::string profilerLog = timeString;
    profilerLog += std::string(size_t(20 - (uint32_t)(timeString.size())), ' ');
    profilerLog += " | ";
    profilerLog += m_functionName;

#if SHOW_PROFILE_IN_MAIN_LOGS
    IK_LOG_INFO(LogModule::Timer, "{0}", profilerLog.c_str());
#endif
    IK_PROFILE_INFO(LogModule::Timer, "{0}", profilerLog.c_str());
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
      s_instance = iknew PerformanceProfiler();
    }
    return s_instance;
  }
  
  void PerformanceProfiler::Destroy()
  {
    ikdelete s_instance;
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
