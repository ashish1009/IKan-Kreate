//
//  ApplicationImpl.hpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#pragma once

#include "Core/Application.hpp"
#include "Core/Layer.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Window.hpp"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "UI/ImGuiLayer.hpp"

namespace IKan
{
  /// This is the Imple class for Core application
  class Application::Impl
  {
  private:
    // Application APIs -------------------------------------------------
    /// @see Application
    void Run();
    /// @see Application
    void HandleEvents(Event& event);
    /// @see Application
    void Close();
    
    /// @see Application
    void PushLayer(const Ref<Layer>& layer);
    /// @see Application
    void PopLayer(const Ref<Layer>& layer);
    
    // Memeber AIs ------------------------------------------------------
    /// This function initializes the Application Imple class
    /// @param appSpec application specification
    void Initialize(const ApplicationSpecification& appSpec);
    /// This function shuts down the Application Imple class
    void Shutdown();
    
    /// This function begin the ImGui Renderer and render ImGui for all the layers and finally ends the imgui rendering
    void ImGuiRender();
    /// This function dispatched in event dispatcher and trigger when window close event evoked
    /// - Parameter window_close_event: Window close event instacnce
    bool WindowClose([[maybe_unused]] WindowCloseEvent& windowCloseEvent);

    /// This function sets the OnUpdate callback
    /// - Parameter func: function pointer
    void SetOnUpdateCallback(const std::function<void(TimeStep)>& func);
    /// This function sets the On Imgui Render callback
    /// - Parameter func: function pointer
    void SetOnImguiRenderCallback(const std::function<void()>& func);

    // Getters -----------------------------------------------------------
    /// This function returns the specification of application
    const ApplicationSpecification& GetSpecification() const;
    /// This function returns the time step of frame
    TimeStep GetTimestep() const;
    /// This fuinction returns the native pointer Window native as void
    void* GetWindowPtr() const;
    /// This fuinction returns the iKan Window Instance as reference
    Window& GetWindow();
    /// This fuinction returns the ImGui Layer pointer Reference
    UI::ImGuiLayer& GetImGuiLayer() const;

    // Member Variables -----------------------------------------------------------------------------------------------
    bool m_isRunning {false};
    ApplicationSpecification m_specification;
    TimeStep m_timeStep;
    LayerStack m_layers;
    Ref<Window> m_window;
    Ref<UI::ImGuiLayer> m_imguiLayer;

    std::function<void(TimeStep)> m_onUpdateCallback;
    std::function<void()> m_onImguiRenderCallback;
    
    friend class Application;
  };
} // namespace IKan
