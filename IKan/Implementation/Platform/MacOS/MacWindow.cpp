//
//  MacWindow.cpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#include "MacWindow.hpp"

#include "Events/ApplicationEvents.h"
#include "Events/MouseEvents.h"
#include "Events/KeyEvents.h"

namespace IKan
{
  MacWindow::MacWindow(const WindowSpecification& windowSpec)
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Window, "Creating MAC OS GLFW Window.");

    // Copy the specification to the window data.
    m_data.specification = windowSpec;

    // Initialize the GLFW Library.
    [[maybe_unused]] bool glfwInitialized = GLFW_TRUE == glfwInit();
    IK_ASSERT(glfwInitialized, "Can not initialise the GLFW library !");
    
    // Configure the GLFW Context Version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 100);
    
    // If flag is true then this removes titlebar.
    glfwWindowHint(GLFW_DECORATED, !m_data.specification.hideTitleBar);

    // Params to create the GLFW window
    int32_t width = static_cast<int32_t>(m_data.specification.width);
    int32_t height = static_cast<int32_t>(m_data.specification.height);
    
    IK_ASSERT(width > 0 or height > 0, "Window size can not be zero !");
    
    GLFWmonitor* primaryMonitor = nullptr;
    GLFWwindow* sharedMonitor = nullptr;
    
    // Overwrite the data if full screen is enabled.
    if (m_data.specification.isFullScreen)
    {
      primaryMonitor = glfwGetPrimaryMonitor();
      const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
      width = mode->width;
      height = mode->height;
    }
    
    // Create the GLFW Window.
    IK_ASSERT(0 != width or 0 != height, "Null width and height of window !");
    m_window = glfwCreateWindow(width, height, m_data.specification.title.data(), primaryMonitor, sharedMonitor);
    
    // If window is not created succesfully then terminate!
    if (nullptr == m_window)
    {
      glfwTerminate();
      IK_ASSERT(false, "Unable to create the window !");
    }
    glfwSwapInterval(1);
    
    // Set the user defined pointer to GLFW Window, this pointer will be retrieved when an interrupt will be triggered.
    glfwSetWindowUserPointer(m_window, &m_data);
    
    // Set GLFW callbacks.
    SetEventCallbacks();
    
    // Window controlls from client.
    if (m_data.specification.startMaximized)
    {
      Maximize();
    }
    SetResizable(m_data.specification.resizable);
    SetAtCenter();
  }
  
  MacWindow::~MacWindow()
  {
    IK_PROFILE();
    IK_LOG_WARN(LogModule::Window, "Destroying MAC OS GLFW Window.");
    
    // Destroy GLFW Window
    glfwTerminate();
    glfwDestroyWindow(m_window);
  }
  
  void MacWindow::SetEventCallbacks()
  {
    IK_PROFILE();
    // Call back for Window Resize Event -----------------------------------------------------------------------------
    glfwSetWindowSizeCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t width,
   [[maybe_unused]] int32_t height
   ) {
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
     WindowCloseEvent event;
     data.eventCallbackFunction(event);
   });
    
    // Call back for Window Focused Event ----------------------------------------------------------------------------
    glfwSetWindowFocusCallback( m_window,
[](
   [[maybe_unused]] GLFWwindow* window,
   [[maybe_unused]] int32_t icontified) {
   });
    
    // Call back for Window Minimize Event ----------------------------------------------------------------------------
    glfwSetWindowIconifyCallback(m_window,
[](GLFWwindow* window, int iconified) {
      MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
      WindowMinimizeEvent event((bool)iconified);
      data.eventCallbackFunction(event);
    });
    
    // Call back for Window Maximized Event ----------------------------------------------------------------------------
    glfwSetWindowIconifyCallback(m_window,
[](GLFWwindow* window, int maximized) {
      MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
      WindowMaximizeEvent event((bool)maximized);
      data.eventCallbackFunction(event);
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
     
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
     
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
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
     MacWindowData& data = *static_cast<MacWindowData*>(glfwGetWindowUserPointer(window));
     MouseMovedEvent event(static_cast<float>(xPosition), static_cast<float>(yPosition));
     data.eventCallbackFunction(event);
   });
  }
  
  void MacWindow::Update()
  {

  }
  
  void MacWindow::Maximize() const
  {
    glfwMaximizeWindow(m_window);
  }
  
  void MacWindow::Restore() const
  {
    glfwRestoreWindow(m_window);
  }
  
  void MacWindow::Iconify() const
  {
    glfwIconifyWindow(m_window);
  }
  
  void MacWindow::SetAtCenter() const
  {
    const GLFWvidmode* videMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const WindowSpecification& spec = m_data.specification;
    int32_t x = (videMode->width / 2) - (static_cast<int32_t>(spec.width) / 2);
    int32_t y = (videMode->height / 2) - (static_cast<int32_t>(spec.height) / 2);
    glfwSetWindowPos(m_window, x, y);
  }
  void MacWindow::SetPosition(const glm::vec2& pos) const
  {
    glfwSetWindowPos(m_window, pos.x, pos.y);
  }
  
  void MacWindow::SetResizable(bool resizable) const
  {
    std::string canCannotString = resizable ? "can" : "cannot";
    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE );
  }
  
  void MacWindow::SetSize(float width, float height)
  {
    if (0 == width or 0 == height or (m_data.specification.width == width and m_data.specification.height == height))
    {
      return;
    }
    
    m_data.specification.width = width;
    m_data.specification.height = height;
    
    glfwSetWindowSize(m_window, m_data.specification.width, m_data.specification.height);
    SetAtCenter();
  }
  
  void MacWindow::SetTitle(const std::string& title)
  {
    m_data.specification.title = title;
    glfwSetWindowTitle(m_window, m_data.specification.title.data());
  }
  
  void MacWindow::SetEventFunction(const EventCallbackFn& eventCallbackFunction)
  {
    m_data.eventCallbackFunction = eventCallbackFunction;
  }
  
  // Getters ---------------------------------------------------------------------------------------------------------
  bool MacWindow::IsActive() const
  {
    return !glfwWindowShouldClose(m_window);
  }
  
  bool MacWindow::IsMaximized() const
  {
    return static_cast<bool>(glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED));
  }
  
  bool MacWindow::IsStartMaximized() const
  {
    return m_data.specification.startMaximized;
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
  
  std::string_view MacWindow::GetTitle() const
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
