//
//  MacWindow.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "MacWindow.hpp"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"

namespace IKan
{
  MacWindow::MacWindow(const Window::Specification& specification)
  {
    IK_PROFILE();
    IK_LOG_TRACE(LogModule::Window, "Creating Window Instance for MAC OS");

    // Copy the specificaiton to the window data
    m_data.specification = specification;

    // Initialise the GLFW Library
    bool glfwInitialised = GLFW_TRUE == glfwInit();
    IK_ASSERT(glfwInitialised, "Can not initialise the GLFW library");
    
    // Configure the GLFW Context Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // If flag is true then this removes titlebar on all platforms and all of the native window effects on
    // non-Windows platforms
    glfwWindowHint(GLFW_DECORATED, !m_data.specification.hideTitleBar);
    
    // Params to create the GLFW window
    int32_t width = static_cast<int32_t>(m_data.specification.width);
    int32_t height = static_cast<int32_t>(m_data.specification.height);
    GLFWmonitor* primaryMonitor = nullptr;
    GLFWwindow* sharedMonitor = nullptr;

    // Overwrite the data if full screen is enabled
    if (m_data.specification.isFullscreen)
    {
      primaryMonitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
      width = mode->width;
      height = mode->height;
    }

    // Create the GLFW Window
    m_window = glfwCreateWindow(width, height, m_data.specification.title.c_str(), primaryMonitor, sharedMonitor);
    
    // If window is not created succesfully then terminate ...
    if (m_window == nullptr)
    {
      glfwTerminate();
      IK_ASSERT(false, "Unable to create the window");
    }
    
#ifndef __APPLE__
    // Setting vSync based on the specification
    glfwSwapInterval(static_cast<int32_t>(m_data.specification.isVsync));
#else
    glfwSwapInterval(1);
#endif

    // Set the User defined pointer to GLFW Window, this pointer will be retrieved when an interrupt will be triggered
    glfwSetWindowUserPointer(m_window, &m_data);
    
    // Set GLFW Callbacks
    SetEventCallbacks();
  }
  
  MacWindow::~MacWindow()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Window, "Destroying Window Instance for MAC OS");
  }
  
  void MacWindow::Shutdown()
  {
    glfwTerminate();
    glfwDestroyWindow(m_window);
    IK_LOG_WARN(LogModule::Window, "Shutting down the MAC OS Window");
  }
  
  void MacWindow::SetEventCallbacks()
  {
    // Call back for Window Resize Event -----------------------------------------------------------------------------
    glfwSetWindowSizeCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t width,
   [[maybe_unused]] int32_t height
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     data.specification.width  = static_cast<uint32_t>(width);
     data.specification.height = static_cast<uint32_t>(height);
     
     WindowResizeEvent event(data.specification.width, data.specification.height);
     data.eventCallbackFunction(event);
   });
    
    // Call back for Window Close Event ------------------------------------------------------------------------------
    glfwSetWindowCloseCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     WindowCloseEvent event;
     data.eventCallbackFunction(event);
   });
    
    // Call back for Window Focused Event ----------------------------------------------------------------------------
    glfwSetWindowFocusCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t icontified) {
   });
    
    // Call back for Set the Key Event -------------------------------------------------------------------------------
    glfwSetKeyCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t key,
   [[maybe_unused]] int32_t scanCode,
   [[maybe_unused]] int32_t action,
   [[maybe_unused]] int32_t mods
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     
     switch (action) {
       case GLFW_PRESS: {
         KeyPressedEvent event(static_cast<Key>(key), 0);
         data.eventCallbackFunction(event);
         break;
       }
       case GLFW_RELEASE: {
         KeyReleasedEvent event(static_cast<Key>(key));
         data.eventCallbackFunction(event);
         break;
       }
       case GLFW_REPEAT: {
         KeyPressedEvent event(static_cast<Key>(key), 1);
         data.eventCallbackFunction(event);
         break;
       }
     }
   });
    
    // Call back for Character pressed Event -------------------------------------------------------------------------
    glfwSetCharCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] uint32_t keyCode
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     KeyTypedEvent event(static_cast<Key>(keyCode));
     data.eventCallbackFunction(event);
   });
    
    // Call back for Mouse button pressed Event ----------------------------------------------------------------------
    glfwSetMouseButtonCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t button,
   [[maybe_unused]] int32_t action,
   [[maybe_unused]] int32_t mods
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     
     switch (action) {
       case GLFW_PRESS: {
         MouseButtonPressedEvent event(static_cast<MouseButton>(button));
         data.eventCallbackFunction(event);
         break;
       }
       case GLFW_RELEASE: {
         MouseButtonReleasedEvent event(static_cast<MouseButton>(button));
         data.eventCallbackFunction(event);
         break;
       }
     }
   });
    
    // Call back for Mouse Scrolled Event ----------------------------------------------------------------------------
    glfwSetScrollCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] double xOffset,
   [[maybe_unused]] double yOffset
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
     data.eventCallbackFunction(event);
   });
    
    // Call back for Mouse Cursor Event ------------------------------------------------------------------------------
    glfwSetCursorPosCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] double xPosition,
   [[maybe_unused]] double yPosition
   ) {
     MacWindow::Data& data = *static_cast<MacWindow::Data*>(glfwGetWindowUserPointer(window));
     MouseMovedEvent event(static_cast<float>(xPosition), static_cast<float>(yPosition));
     data.eventCallbackFunction(event);
   });
  }

} // namespace IKan
