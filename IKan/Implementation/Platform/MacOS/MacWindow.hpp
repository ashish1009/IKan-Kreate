//
//  MacWindow.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include <GLFW/glfw3.h>
#include "Core/CoreWindow.hpp"

namespace IKan
{
  /// This is the implementation class for creating the window instance for MAC OS
  class MacWindow : public Window
  {
  public:
    /// This function stores the Mac Window Data
    struct Data
    {
      Specification specification;
      EventCallbackFn eventCallbackFunction;
    };

    /// This is the constructor to create the window fror MAC OS
    /// - Parameter specification: Window specification provided by user
    MacWindow(const Window::Specification& specification);
    /// This is the default destructor of MAC Window
    ~MacWindow();
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function shudown the GLFW Window
    void Shutdown();
    /// This function sets the event callback fucntions to window data
    void SetEventCallbacks();
    
    // Member Variables ----------------------------------------------------------------------------------------------
    float m_lastFrameTime = 0.0f;
    GLFWwindow* m_window;
    Data m_data;
    TimeStep m_timeStep;
  };
} // namespace IKan
