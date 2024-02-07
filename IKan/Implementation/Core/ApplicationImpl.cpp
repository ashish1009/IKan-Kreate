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
    // Updating all the attached layer
    for (auto& layer : m_layers)
    {
      layer->OnUpdate(m_timeStep);
    }
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
    m_layers.PushLayer(layer);
    IK_PROFILE();
  }
  
  void Application::Impl::PopLayer(const Ref<Layer>& layer)
  {
    m_layers.PopLayer(layer);
    IK_PROFILE();
  }
} // namespace IKan
