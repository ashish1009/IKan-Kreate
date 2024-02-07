//
//  ApplicationImpl.cpp
//  IKan
//
//  Created by Ashish . on 07/02/24.
//

#include "ApplicationImpl.hpp"

namespace IKan
{
  void Application::Impl::Run()
  {
    IK_PERFORMANCE("Application::Run");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                          Starting Game Loop                              ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    while (m_isRunning)
    {
      // Updating all the attached layer
      for (auto& layer : m_layers)
      {
        layer->OnUpdate(m_timeStep);
      }
    }
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
    IK_LOG_INFO("", "                           Ending Game Loop                               ");
    IK_LOG_INFO("", "--------------------------------------------------------------------------");
  }
  
  void Application::Impl::HandleEvents(Event &event)
  {
    // Event Handler for all layers
    for (auto& layer : m_layers)
    {
      layer->OnEvent(event);
    }
  }
  
  void Application::Impl::Close()
  {
    IK_PROFILE();
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
} // namespace IKan
