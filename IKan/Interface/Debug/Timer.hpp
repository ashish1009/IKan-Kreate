//
//  Timer.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

namespace IKan
{
  // Timer ------------------------------------------------------------------------------------------------------------
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
    double Elapsed() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in mili seconds
    double ElapsedMiliSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in micro seconds
    double ElapsedMicroSeconds() const;
    /// This function returns the elapsed time since Timer instance created or Reset API is called in nano seconds
    double ElapsedNanoSeconds() const;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(Timer);
    
  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
  };
  
  // ScopedTimer ------------------------------------------------------------------------------------------------------
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
#define IK_PROFILE() IKan::ScopedTimer _scopedTimer__LINE__(__PRETTY_FUNCTION__);

} // namespace IKan
