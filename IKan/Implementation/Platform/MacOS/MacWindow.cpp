//
//  MacWindow.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "MacWindow.hpp"

namespace IKan
{
  MacWindow::MacWindow(const Window::Specification& specification)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Window, "Creating Window Instance for MAC OS");
  }
  
  MacWindow::~MacWindow()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Window, "Destroying Window Instance for MAC OS");
  }
} // namespace IKan
