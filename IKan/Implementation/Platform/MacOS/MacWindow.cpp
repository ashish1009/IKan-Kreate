//
//  MacWindow.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "MacWindow.hpp"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

namespace IKan
{
  MacWindow::MacWindow(const WindowSpecification& windowSpec)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Window, "Creating MAC OS GLFW Window");
    
    // Copy the specificaiton to the window data
    m_data.specification = windowSpec;
  }
  
  MacWindow::~MacWindow()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Window, "Destroying MAC OS GLFW Window");
  }
  
  void MacWindow::Update()
  {
    IK_PERFORMANCE("MacWindow::Update");
  }
  
  void MacWindow::Maximize() const
  {
    
  }
  
  void MacWindow::Restore() const
  {
    
  }
  
  void MacWindow::Iconify() const
  {
    
  }
  
  void MacWindow::SetAtCenter() const
  {
    
  }
  void MacWindow::SetPosition(const glm::vec2& pos) const
  {
    
  }
  
  void MacWindow::SetResizable(bool resizable) const
  {
    
  }
  
  void MacWindow::SetTitle(const std::string& title)
  {
    
  }
  
  void MacWindow::SetEventFunction(const EventCallbackFn& eventCallbackFunction)
  {
    
  }
  
  // Getters ---------------------------------------------------------------------------------------------------------
  bool MacWindow::IsActive() const
  {
    return false;
  }
  
  bool MacWindow::IsMaximized() const
  {
    return false;
  }
  
  bool MacWindow::IsTitleBarHidden() const
  {
    return m_data.specification.hideTitleBar;
  }
  
  bool MacWindow::IsFullscreen() const
  {
    return m_data.specification.isFullScreen;
  }
  
  uint32_t MacWindow::GetWidth() const
  {
    return m_data.specification.width;
  }
  
  uint32_t MacWindow::GetHeight() const
  {
    return m_data.specification.height;
  }
  
  const std::string& MacWindow::GetTitle() const
  {
    return m_data.specification.title;
  }
  
  TimeStep MacWindow::GetTimestep() const
  {
    return m_timeStep;
  }
  
  void* MacWindow::GetNativeWindow() const
  {
    return static_cast<void*>(m_window);
  }
} // namespace IKan
