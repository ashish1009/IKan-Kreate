//
//  TimeStep.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "TimeStep.hpp"

namespace IKan
{
  TimeStep::TimeStep(float time)
  : m_time(time)
  {
    
  }
  
  TimeStep::operator float() const
  {
    return m_time;
  }
  
  float TimeStep::Seconds() const
  {
    return m_time;
  }
  
  float TimeStep::MilliSeconds() const
  {
    return Seconds() * 1000.0f;
  }
  
  float TimeStep::MicroSeconds() const
  {
    return MilliSeconds() * 1000.0f;
  }
  
  float TimeStep::FPS() const
  {
    return (float)1.0f / m_time;
  }
} // namespace IKan
