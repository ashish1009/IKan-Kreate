//
//  Timer.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

namespace IKan
{
#define SHOW_PROFILE_IN_MAIN_LOGS 0
  /// This class starts the timeer on instantiation and calculates the elapsed time. Elapsed time can be checked
  /// with APIs Elapsed(), ElapsedMiliSeconds() ....
  class Timer
  {
  public:
    /// This Constructor creates the instacne and starts a new Timer
    Timer();
    
    /// This function resets the timer to 0
    void Reset();
    /// This function returns the elapsed time since Timer instance created or Reset API is called in seconds
    float Elapsed() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in mili seconds
    float ElapsedMiliSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in micro seconds
    float ElapsedMicroSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in nano seconds
    float ElapsedNanoSeconds() const;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Timer);
    
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  };

  /// This function starts the timer on creating instance of Scoped timer. Shoes the elapsed time of scope where
  /// ScopedTimer is initialised.
  /// - Intantiate the Profiler with function name in any scope. It will print the executrion time of the function or
  /// - Just call the Macro defiled PROFILE().
  /// - Important: Scope should be inside Log::Init() and Log::Shutdown()
  class ScopedTimer
  {
  public:
    /// This Constructor instantiate the function name that need to be profiled
    /// - Parameter - Function name to be profiled:
    ScopedTimer(const char* functionName);
    /// Profiler destructor. Shows the results of profile of a scope
    ~ScopedTimer();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedTimer);
    
  private:
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    std::chrono::time_point<std::chrono::steady_clock> m_endTime;
    std::chrono::duration<float> m_duration;
    const char* m_functionName;
  };

  // Call 'Profile()' in begining of any scope. It will compute the time and print
  // the Profiler result at the end of Scope
#define IK_PROFILE() \
IKan::ScopedTimer _scopedTimer__LINE__(__PRETTY_FUNCTION__); \

  /// This function store the runtime preformance of profiler
  class PerformanceProfiler
  {
  public:
    /// This function set the frame timing
    /// - Parameters:
    ///   - name: function name
    ///   - time: time
    void SetPerFrameTiming(const char* name, float time);
    /// This function clear the profiler
    void Clear();
    /// This function returs the performance data
    const std::unordered_map<const char*, float>& GetPerFrameData() const;
    
    /// This function returns the singleton instance of Performance profiler
    static PerformanceProfiler* Get();
    /// This function destroy the singleton instance of Performance profiler
    static void Destroy();
    
  private:
    inline static PerformanceProfiler* s_instance;
    std::unordered_map<const char*, float> m_perFrameData;
  };
  
  /// This function store the preformance of profiler scoped
  class ScopePerfTimer
  {
  public:
    ScopePerfTimer(const char* name, PerformanceProfiler* profiler);
    ~ScopePerfTimer();
    
  private:
    const char* m_name;
    PerformanceProfiler* m_profiler;
    Timer m_timer;
  };

#define IK_PERFORMANCE(name) \
IKan::ScopePerfTimer timer__LINE__(name, IKan::PerformanceProfiler::Get());
  
#define IK_PERFORMANCE_FUN() \
IKan::ScopePerfTimer timer__LINE__(__PRETTY_FUNCTION__, IKan::PerformanceProfiler::Get());

} // namespace IKan
