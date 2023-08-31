//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 27/07/23.
//

#include "Application.hpp"
#include "Renderer/RendererStats.hpp"
#include "Asset/AssetManager.hpp"
#include "Editor/AssetEdtitor.hpp"
#include "UI/ImGuiTheme.hpp"

namespace IKan
{
  Application::Application(const Specification& spec)
  : m_specificaion(spec), m_layers(CreateScope<LayerStack>())
  {
    IK_PROFILE();

    IK_ASSERT(!s_instance, "Application instance already created");
    // Copy the single instance of application
    s_instance = this;

    IK_LOG_TRACE(LogModule::Application, "Creating Core Application Instance : {0}", m_specificaion.name);
    
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                     Core Application Initialized                         ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  Application::~Application()
  {
    IK_PROFILE();

    IK_LOG_WARN(LogModule::Application, "Destroying Core Application Instance : {0}", m_specificaion.name);
  }

  void Application::Run()
  {
    {
      IKan::ScopedTimer onShutdown("Client Application Initialize()");
      // Initialize the client application
      OnInit();
    }
    // GAME LOOP -----------------------------------------------------------------------------------------------------
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
        
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                           Ending Game Loop                               ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");

    {
      IKan::ScopedTimer onShutdown("Client Application Shutdown()");
      // Shutdown the client application
      OnShutdown();
    }
  }
  
  void Application::HandleEvents(Event &event)
  {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowCloseEvent>(IK_BIND_EVENT_FN(Application::WindowClose));
        
    // Event Handler for all layers
    for (auto& layer : *(m_layers.get()))
    {
      layer->OnEvent(event);
    }
    
    // Client side event handler funciton
    OnEvent(event);    
  }
  
  bool Application::WindowClose([[maybe_unused]] WindowCloseEvent& window_close_event)
  {
    Close();
    return false;
  }
  
  void Application::Close()
  {
    m_isRunning = false;
    IK_LOG_WARN(LogModule::Application, "Closing the Application");
  }
  
  void Application::ImGuiRender()
  {

  }
  
  void Application::PushLayer(const Ref<Layer> &layer)
  {
    m_layers->PushLayer(layer);
  }
  
  void Application::PopLayer(const Ref<Layer> &layer)
  {
    m_layers->PopLayer(layer);
  }
  
  const Application::Specification& Application::GetSpecification() const
  {
    return m_specificaion;
  }

  void* Application::GetWindowPtr() const
  {
    return m_window->GetNativeWindow();
  }
  
  Window& Application::GetWindow()
  {
    return *(m_window.get());
  }
  TimeStep Application::GetTimestep() const
  {
    return m_timeStep;
  }
  UI::ImGuiLayer& Application::GetImGuiLayer() const
  {
    return *m_imguiLayer.get();
  }

  Application& Application::Get()
  {
    return *s_instance;
  }
  
} // namespace IKan
