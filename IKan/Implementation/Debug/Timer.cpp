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
} // namespace IKan
