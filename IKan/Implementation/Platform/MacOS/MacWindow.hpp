//
//  MacWindow.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Core/CoreWindow.hpp"

namespace IKan
{
  /// This is the implementation class for creating the window instance for MAC OS
  class MacWindow : public Window
  {
  public:
    /// This is the constructor to create the window fror MAC OS
    /// - Parameter specification: Window specification provided by user
    MacWindow(const Window::Specification& specification);
    /// This is the default destructor of MAC Window
    ~MacWindow();
  };
} // namespace IKan
