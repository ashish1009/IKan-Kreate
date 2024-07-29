//
//  Window.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include "Events/Events.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This structure stores the window specification data
  struct WindowSpecification
  {
    std::string title {"IKan"};
    uint32_t width {1600}, height {900};
    bool isFullScreen {false};
    bool hideTitleBar {false};
  };

  /// This class is the abstract for GLFW window. Provides the APIs for using GLFW window data
  /// - Note: Implementation depends on supported platform
  class Window
  {
  public:
    /// Default virtual destructor
    virtual ~Window() = default;
    
    // Fundamentals ---------------------------------------------------------------------------------------------------
    /// This function updates the window for each frame. This function to be called by application inside game loop.
    /// It swaps the context buffers. Also updates the time step of each frame.
    /// - Note: To be called in main thread only.
    virtual void Update() = 0;
    /// This function updates the event callback functions and register it to the window instance.
    /// - Parameter eventCallbackFn: function pointer to be registered in the window.
    /// - Note: EventCallbackFn to be called every time any even triggered to window.
    virtual void SetEventFunction(const EventCallbackFn& eventCallbackFn) = 0;

    // Controllers ----------------------------------------------------------------------------------------------------
    /// This function maximises the window to its full length based on the current monitor.
    virtual void Maximize() const = 0;
    /// This function restores the window to its original length based on the current monitor and last size of window.
    virtual void Restore() const = 0;
    /// This function minimizes the window.
    virtual void Iconify() const = 0;
    /// This function places the window at the center of the screen.
    virtual void SetAtCenter() const = 0;
    /// This function sets the position of window.
    /// - Parameter pos: position of window.
    virtual void SetPosition(const glm::vec2& pos) const = 0;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API.
    /// - Parameter resizable: new flag as resizable.
    virtual void SetResizable(bool resizable) const = 0;
    /// This function resizes the window
    /// - Parameters:
    ///  - width: new width of window
    ///  - height: new height of window
    virtual void SetSize(float width, float height) = 0;
    /// This function chnages the title of the window as 'newTitle'.
    /// - Parameter newTitle: new title of the window.
    virtual void SetTitle(const std::string& newTitle) = 0;
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function checks is window maximized.
    virtual bool IsMaximized() const = 0;
    /// This function returns is window active
    virtual bool IsActive() const = 0;
    /// This function returns is window titlebar hidden.
    virtual bool IsTitleBarHidden() const = 0;
    /// This function returns is window in full screen mode.
    virtual bool IsFullscreen() const = 0;
    /// This function returns the width of window.
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the height of window.
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the current time step of this frame.
    virtual TimeStep GetTimestep() const = 0;
    /// This function returns the native window pointer (void*).
    virtual void* GetNativeWindow() const = 0;
    /// This function returns the title of the window.
    virtual std::string_view GetTitle() const = 0;
  };
  
  /// This structure stores the API to create the window instance based on the supported platform
  struct WindowFactory
  {
    /// This function creates the window instance based on the window specification data and supported platform
    /// - Parameter specification: window specification data
    [[nodiscard("Created window should not be discarded")]] static Scope<Window> Create(const WindowSpecification& specification);
  };
} // namespace IKan
