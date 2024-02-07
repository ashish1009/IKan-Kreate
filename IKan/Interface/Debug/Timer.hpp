//
//  Timer.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

namespace IKan
{
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
} // namespace IKan
