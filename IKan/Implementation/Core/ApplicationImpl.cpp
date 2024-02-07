//
//  ApplicationImpl.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "ApplicationImpl.hpp"

namespace IKan
{
  void Application::Impl::Initialize(const ApplicationSpecification& appSpec)
  {
    IK_PROFILE();
    
    // Copy the Specificaion
    m_specification = appSpec;
    
    // Create the window
    m_window = WindowFactory::Create(m_specification.windowSpecificaion);
    
    // Set the application callback to window
    m_window->SetEventFunction(IK_BIND_EVENT_FN(Application::Impl::HandleEvents));
    
    // Control Window
    m_window->SetResizable(m_specification.resizable);
    if (m_specification.startMaximized)
    {
      m_window->Maximize();
    }
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                     Core Application Initialized                         ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  void Application::Impl::Shutdown()
  {
    IK_PROFILE();

    // Reset the window
    m_window.reset();
  }
  
  void Application::Impl::Run()
  {
    IK_PERFORMANCE("Application::Run");
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    while (m_isRunning)
    {
      IK_PERFORMANCE("Application::Run");
      // Update the window swap buffers
      m_window->Update();
      
      // Store the frame time difference
      m_timeStep = m_window->GetTimestep();

      // Updating all the attached layer
      for (auto& layer : m_layers)
      {
        layer->OnUpdate(m_timeStep);
      }
      
      // Render the Gui for Application
      ImGuiRender();
      
      m_onUpdateCallback(m_timeStep);
    }
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                           Ending Game Loop                               ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  void Application::Impl::HandleEvents(Event &event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::Impl::WindowClose));

    // Event Handler for all layers
    for (auto& layer : m_layers)
    {
      layer->OnEvent(event);
    }
  }
  
  bool Application::Impl::WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event)
  {
    IK_PROFILE();
    Close();
    return false;
  }

  void Application::Impl::ImGuiRender()
  {
    // Render Imgui for all layers
    for (auto& layer : m_layers)
    {
      layer->OnImGuiRender();
    }
    
    m_onImguiRenderCallback();
  }

  void Application::Impl::Close()
  {
    IK_PROFILE();
    m_isRunning = false;
    IK_LOG_TRACE(LogModule::Application, "Closing the Application");
  }
  
  void Application::Impl::PushLayer(const Ref<Layer>& layer)
  {
    IK_PROFILE();
    m_layers.PushLayer(layer);
  }
  
  void Application::Impl::PopLayer(const Ref<Layer>& layer)
  {
    IK_PROFILE();
    m_layers.PopLayer(layer);
  }
  
  void Application::Impl::SetOnUpdateCallback(const std::function<void(TimeStep)>& func)
  {
    m_onUpdateCallback = func;
  }
  
  void Application::Impl::SetOnImguiRenderCallback(const std::function<void()>& func)
  {
    m_onImguiRenderCallback = func;
  }
  
  const ApplicationSpecification& Application::Impl::GetSpecification() const
  {
    return m_specification;
  }
  
  TimeStep Application::Impl::GetTimestep() const
  {
    return m_timeStep;
  }
  
  void* Application::Impl::GetWindowPtr() const
  {
    return m_window->GetNativeWindow();
  }
  
  Window& Application::Impl::GetWindow()
  {
    return *(m_window.get());
  }
} // namespace IKan
