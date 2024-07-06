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
} // namespace IKan
