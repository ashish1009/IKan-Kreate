//
//  Application.cpp
//  IKan
//
//  Created by Ashish . on 06/02/24.
//

#include "Application.hpp"
#include "Core/ApplicationImpl.hpp"

namespace IKan
{
  Application::Application(const ApplicationSpecification& appSpec)
  {
    IK_ASSERT(!s_instance, "Application instance already created");
    s_instance = this;
    
    IK_LOG_INFO(LogModule::Application, "Initializing Core application data");
    m_applicationImpl->Initialize(appSpec);
    m_applicationImpl->SetOnUpdateCallback([this](TimeStep ts) { OnUpdate(ts); });
    m_applicationImpl->SetOnImguiRenderCallback([this]() { OnImGuiRender(); });
  }
  
  Application::~Application()
  {
    IK_LOG_INFO(LogModule::Application, "Destroying Core application data");
    m_applicationImpl->Shutdown();
  }
  
  void Application::Run()
  {
    OnInit();
    m_applicationImpl->Run();
    OnShutdown();
  }
  
  void Application::HandleEvents(Event &event)
  {
    m_applicationImpl->HandleEvents(event);

    // Client side event handler funciton
    OnEvent(event);
  }
  
  void Application::Close()
  {
    m_applicationImpl->Close();
  }
  
  void Application::PushLayer(const Ref<Layer>& layer)
  {
    m_applicationImpl->PushLayer(layer);
  }
  
  void Application::PopLayer(const Ref<Layer>& layer)
  {
    m_applicationImpl->PopLayer(layer);
  }

  const ApplicationSpecification& Application::GetSpecification() const
  {
    return m_applicationImpl->GetSpecification();
  }
  
  TimeStep Application::GetTimestep() const
  {
    return m_applicationImpl->GetTimestep();
  }
  
  void* Application::GetWindowPtr() const
  {
    return m_applicationImpl->GetWindowPtr();
  }
  
  Window& Application::GetWindow()
  {
    return m_applicationImpl->GetWindow();
  }

  Application& Application::Get()
  {
    return *s_instance;
  }
} // namespace IKan
