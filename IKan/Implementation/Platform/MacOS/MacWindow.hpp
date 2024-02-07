//
//  MacWindow.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include <GLFW/glfw3.h>
#include "Core/Window.hpp"

namespace IKan
{
  /// This struction stores the mac window data
  struct MacWindowData
  {
    WindowSpecification specification;
    EventCallbackFn eventCallbackFunction;
  };

  /// This is the implementation class for MAC GLFW Window
  class MacWindow : public Window
  {
  public:
    /// This is the constructor to create the window fror MAC OS
    /// - Parameter windowSpec: Window specification provided by user
    MacWindow(const WindowSpecification& windowSpec);
    /// This is the default destructor of MAC Window
    ~MacWindow();
    
    // Fundamentals ---------------------------------------------------------------------------------------------------
    /// This function updates the window for each frame. This function to be called by application on each frame.
    /// It swap the context buffers. Also update the time of each frame
    void Update() override;
    /// This function updates the event callback function and register it to the window instance
    /// - Parameter eventCallbackFn: function pointer to be registered in the window
    /// - Important: EventCallbackFn to be called every time any even triggered to window
    void SetEventFunction(const EventCallbackFn& eventCallbackFn) override;
    
    // Controllers ----------------------------------------------------------------------------------------------------
    /// This function maximise the window to its full length based on the current monitor. Call when Window need to be
    /// maximised
    void Maximize() const override;
    /// This function restore the window to its original length based on the current monitor.
    void Restore() const override;
    /// This function Minimize the window
    void Iconify() const override;
    /// This function places the window at the center of the screen
    void SetAtCenter() const override;
    /// This function sets the posittion of window
    /// - Parameter pos: position of window
    void SetPosition(const glm::vec2& pos) const override;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API
    /// - Parameter resizable: new flag as resizable
    void SetResizable(bool resizable) const override;
    /// This function chnages the title of the window as 'newTitle'
    /// - Parameter newTitle: new title of the window
    void SetTitle(const std::string& newTitle) override;
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function checks is window maximized
    bool IsMaximized() const override;
    /// This function returns is window is active
    bool IsActive() const override;
    /// This function returns is window is decoarated. When decorated then removes titlebar from the window
    bool IsTitleBarHidden() const override;
    /// This function returns is the window is full screen
    bool IsFullscreen() const override;
    /// This function returns the width of window
    uint32_t GetWidth() const override;
    /// This function returns the height of window
    uint32_t GetHeight() const override;
    /// This function returns the current time step of this frame
    TimeStep GetTimestep() const override;
    /// This function returns the native window pointer (void*)
    void* GetNativeWindow() const override;
    /// This function returns the title of the window
    const std::string& GetTitle() const override;
    
  private:
    // Member Functions -----------------------------------------------------------------------------------------------
    /// This function sets the event callback fucntions to window data
    void SetEventCallbacks();

    // Member Variables -----------------------------------------------------------------------------------------------
    float m_lastFrameTime = 0.0f;
    MacWindowData m_data;
    GLFWwindow* m_window;
    TimeStep m_timeStep;
  };
} // namespace IKan
