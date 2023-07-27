//
//  CoreWindow.hpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#pragma once

#include "Event/Event.h"
#include "Core/TimeStep.hpp"

namespace IKan
{
  /// This is the Interface class for creating window
  class Window
  {
  public:
    /// This structure stores the Window specificaion
    struct Specification
    {
      std::string title = "IKan Window";
      uint32_t width = 1600, height = 900;
      
      bool isFullscreen = false;
      bool hideTitleBar = false;
      
#ifndef __APPLE__
      bool isVsync = true;
#endif
    };
    
    /// This is the default virtual destructor for window
    virtual ~Window() = default;
    
    // Fundamentals --------------------------------------------------------------------------------------------------
    /// This function updates the window for each frame. This function to be called by application on each frame.
    /// It swap the context buffers. Also update the time of each frame
    virtual void Update() = 0;
    /// This function updates the event callback function and register it to the window instance
    /// - Parameter eventCallbackFn: function pointer to be registered in the window
    /// - Important: EventCallbackFn to be called every time any even triggered to window
    virtual void SetEventFunction(const EventCallbackFn& eventCallbackFn) = 0;
    
    // Controllers ---------------------------------------------------------------------------------------------------
    /// This function maximise the window to its full length based on the current monitor. Call when Window need to be
    /// maximised
    virtual void Maximize() const = 0;
    /// This function restore the window to its original length based on the current monitor.
    virtual void Restore() const = 0;
    /// This function Minimize the window
    virtual void Iconify() const = 0;
    /// This function places the window at the center of the screen
    virtual void SetAtCenter() const = 0;
    /// This function sets the posittion of window
    /// - Parameter pos: position of window
    virtual void SetPosition(const glm::vec2& pos) const = 0;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API
    /// - Parameter resizable: new flag as resizable
    virtual void SetResizable(bool resizable) const = 0;
    /// This function chnages the title of the window as 'newTitle'
    /// - Parameter newTitle: new title of the window
    virtual void SetTitle(const std::string& newTitle) = 0;
    
    // Getters -------------------------------------------------------------------------------------------------------
    /// This function checks is window maximized
    virtual bool IsMaximized() const = 0;
    /// This function returns is window is active
    virtual bool IsActive() const = 0;
    /// This function returns is window is decoarated. When decorated then removes titlebar from the window
    virtual bool IsTitleBarHidden() const = 0;
    /// This function returns is the window is full screen
    virtual bool IsFullscreen() const = 0;
    /// This function returns the width of window
    virtual uint32_t GetWidth() const = 0;
    /// This function returns the height of window
    virtual uint32_t GetHeight() const = 0;
    /// This function returns the current time step of this frame
    virtual TimeStep GetTimestep() const = 0;
    /// This function returns the native window pointer (void*)
    virtual void* GetNativeWindow() const = 0;
    /// This function returns the title of the window
    virtual std::string GetTitle() const = 0;
    
    // Static Create API ---------------------------------------------------------------------------------------------
    /// This function creates the window instance based on the operating system supported by IKan Engine
    /// - Parameter specification: Window specification
    [[nodiscard]] static Ref<Window> Create(const Specification& specification);
  };
} // namespace IKan
