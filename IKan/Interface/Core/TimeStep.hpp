//
//  TimeStep.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

namespace IKan
{
  /// This class is used to store the time difference between two renderer Frame
  /// - Note: This class only store the time stamp, it is not storing any counter or difference. Need to update
  ///         manually either by Window or Application each frame
  class TimeStep
  {
  public:
    /// This constructor creates the instance and store the time
    /// - Parameter time: Initial time to be set:
    TimeStep(float time = 0.0f);
    
    /// This function returns the Time as float
    operator float() const;
    
    /// This function returns the time step (Time took to render 1 Frame) in Seconds
    float Seconds() const;
    /// This function returns the time step (Time took to render 1 Frame) in mili Seconds
    float MilliSeconds() const;
    /// This function returns the time step (Time took to render 1 Frame) in micro Seconds
    float MicroSeconds() const;
    
    /// This function returns the frames per seconds based on time step of frame
    float FPS() const;

  private:
    float m_time {0.0f};
  };
} // namespace IKan
