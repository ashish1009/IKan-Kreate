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
    IK_PROFILE();
    IK_ASSERT(!s_instance, "Application instance already created");
    s_instance = this;
    
    IK_LOG_INFO(LogModule::Application, "Initializing Core application data");
  }
  Application::~Application()
  {
    IK_PROFILE();
    IK_LOG_INFO(LogModule::Application, "Destroying Core application data");
  }
  
  void Application::Run()
  {
    m_applicationImpl->Run();
  }
  
  void Application::HandleEvents(Event &event)
  {
    m_applicationImpl->HandleEvents(event);
  }
  
  void Application::Close()
  {
    IK_PROFILE();
    m_applicationImpl->Close();
  }
  
  void Application::PushLayer(const Ref<Layer>& layer)
  {
    IK_PROFILE();
    m_applicationImpl->PushLayer(layer);
  }
  void Application::PopLayer(const Ref<Layer>& layer)
  {
    IK_PROFILE();
    m_applicationImpl->PopLayer(layer);
  }

} // namespace IKan
