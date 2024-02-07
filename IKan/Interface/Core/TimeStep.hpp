//
//  TimeStep.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

namespace IKan
{
  /// This class stores the time difference between two renderer Frame
  /// - Note: This class only store the time stamp, it is not storing any counter or difference. Need to update
  ///         manually either by Window or Application
  class TimeStep
  {
  public:
    /// This constructor creates the instance and store the time
    /// - Parameter time: Initial time to be set:
    TimeStep(double time = 0.0f);
    /// This is the Defatult construtor for Timestep
    ~TimeStep() = default;
    
    /// This function returns the Time as float
    operator double() const;
    /// This function returns the Time as float
    operator float() const;

    /// This function returns the time step (Time took to render 1 Frame) in Seconds
    double Seconds() const;
    /// This function returns the time step (Time took to render 1 Frame) in mili Seconds
    double MilliSeconds() const;
    /// This function returns the time step (Time took to render 1 Frame) in micro Seconds
    double MicroSeconds() const;
    
    /// This function returns the FPS
    double FPS() const;
    
  private:
    double m_time = 0.0f;
  };
} // namespace IKan
