//
//  TimeStep.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "TimeStep.hpp"

namespace IKan
{
  TimeStep::TimeStep(double time)
  : m_time(time)
  {
    
  }
  
  TimeStep::operator double() const
  {
    return m_time;
  }
  
  TimeStep::operator float() const
  {
    return m_time;
  }

  double TimeStep::Seconds() const
  {
    return m_time;
  }
  
  double TimeStep::MilliSeconds() const
  {
    return Seconds() * 1000.0f;
  }
  
  double TimeStep::MicroSeconds() const
  {
    return MilliSeconds() * 1000.0f;
  }
  
  double TimeStep::FPS() const
  {
    return (double)1.0f / m_time;
  }
} // namespace IKan
