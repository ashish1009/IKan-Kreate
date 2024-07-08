//
//  MacWindow.hpp
//  IKan
//
//  Created by Ashish . on 08/07/24.
//

#pragma once

#include <GLFW/glfw3.h>

#include "Core/Window.hpp"
#include "Renderer/Graphics/RendererContext.hpp"

namespace IKan
{
  /// This structure stores the MAC window data.
  /// - Note: This data to be passed in glfw callback
  struct MacWindowData
  {
    WindowSpecification specification;
    EventCallbackFn eventCallbackFunction;
  };
  
  /// This class implements the APIs for creating GLFW Window for MAC platform.
  class MacWindow : public Window
  {
  public:
    /// This constructor creates the instance for GLFW Mac window
    /// - Parameter specification: window specification data
    MacWindow(const WindowSpecification& specification);
    /// This destructor destorys the GLFW Mac window instance
    ~MacWindow();
    
    // Fundamentals ---------------------------------------------------------------------------------------------------
    /// This function updates the window for each frame. This function to be called by application inside game loop.
    /// It swaps the context buffers. Also updates the time step of each frame.
    /// - Note: To be called in main thread only.
    void Update() override;
    /// This function updates the event callback functions and register it to the GLFW window instance.
    /// - Parameter eventCallbackFn: function pointer to be registered in the window.
    /// - Note: EventCallbackFn to be called every time any even triggered to window.
    void SetEventFunction(const EventCallbackFn& eventCallbackFn) override;
    
    // Controllers ----------------------------------------------------------------------------------------------------
    /// This function maximises the window to its full length based on the current monitor.
    void Maximize() const override;
    /// This function restores the window to its original length based on the current monitor and last size of window.
    void Restore() const override;
    /// This function minimizes the window.
    void Iconify() const override;
    /// This function places the window at the center of the screen.
    void SetAtCenter() const override;
    /// This function sets the position of window.
    /// - Parameter pos: position of window.
    void SetPosition(const glm::vec2& pos) const override;
    /// This function makes Window resizable if 'resizable' is true else Window can not be resized after calling this API.
    /// - Parameter resizable: new flag as resizable.
    void SetResizable(bool resizable) const override;
    /// This function resizes the window
    /// - Parameters:
    ///  - width: new width of window
    ///  - height: new height of window
    void SetSize(float width, float height) override;
    /// This function chnages the title of the window as 'newTitle'.
    /// - Parameter newTitle: new title of the window.
    void SetTitle(const std::string& newTitle) override;
    
    // Getters --------------------------------------------------------------------------------------------------------
    /// This function checks is window maximized.
    bool IsMaximized() const override;
    /// This function checks is window maximized at start.
    bool IsStartMaximized() const override;
    /// This function returns is window actived.
    bool IsActive() const override;
    /// This function returns is window titlebar hidden.
    bool IsTitleBarHidden() const override;
    /// This function returns is window in full screen mode.
    bool IsFullscreen() const override;
    /// This function returns the width of window.
    uint32_t GetWidth() const override;
    /// This function returns the height of window.
    uint32_t GetHeight() const override;
    /// This function returns the current time step of this frame.
    TimeStep GetTimestep() const override;
    /// This function returns the native window pointer (void*).
    void* GetNativeWindow() const override;
    /// This function returns the title of the window.
    std::string_view GetTitle() const override;
    
  private:
    // Member Functions -----------------------------------------------------------------------------------------------
    /// This function registers the event callbacks in window.
    void SetEventCallbacks();
    
    // Member Variables -----------------------------------------------------------------------------------------------
    double m_lastFrameTime {0.0f};
    MacWindowData m_data;
    GLFWwindow* m_window;
    TimeStep m_timeStep;
    Scope<RendererContext> m_context;
  };
} // namespace IKan
