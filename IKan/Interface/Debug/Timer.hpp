//
//  Timer.hpp
//  IKan
//
//  Created by Ashish . on 06/07/24.
//

#pragma once

#include <chrono>

namespace IKan
{
  // Timer -----------------------------------------------------------------------------------------------------------
  /// This class starts the timer on instantiation and calculates the elapsed time on destructor calls
  /// - Note: Elapsed time can be checked with APIs Elapsed(), ElapsedMiliSeconds(), ElapsedMicroSeconds() and ElapsedNanoSeconds()
  class Timer
  {
  public:
    /// This Constructor creates the instacne and starts a new Timer
    Timer();
    
    /// This function resets the timer to 0
    void Reset();
    /// This function returns the elapsed time since Timer instance created or Reset API is called in seconds
    double Elapsed() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in mili seconds
    double ElapsedMiliSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in micro seconds
    double ElapsedMicroSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in nano seconds
    double ElapsedNanoSeconds() const;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Timer);
    
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
  };
  
  // ScopedTimer ------------------------------------------------------------------------------------------------------
  /// This class starts the timer on creating instance of Scoped timer. Shows the elapsed time of scope where
  /// ScopedTimer is initialised.
  /// - Instantiate the Profiler with function name in any scope. It will print the execution time of the function or
  ///   Just call the Macro PROFILE().
  /// - Important: Scope should be inside called after Profile Logger is created
  class ScopedTimer
  {
  public:
    /// This Constructor instantiate the function name that need to be profiled
    /// - Parameter - Function name to be profiled
    ScopedTimer(const char* functionName);
    /// This destructor shows the results of profile of a scope
    ~ScopedTimer();
    
    DELETE_COPY_MOVE_CONSTRUCTORS(ScopedTimer);
    
  private:
    std::chrono::time_point<std::chrono::steady_clock> m_endTime;
    std::chrono::duration<float> m_duration;
    
    const std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    const char* m_functionName;
  };
#define IK_PROFILE() IKan::ScopedTimer _scopedTimer__LINE__(__PRETTY_FUNCTION__);
#define IK_NAMED_PROFILE(name) IKan::ScopedTimer _scopedTimer__LINE__(name);

} // namespace IKan
