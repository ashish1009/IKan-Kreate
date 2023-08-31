//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"

namespace Kreator
{
  
  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }
  
  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer")
  {
  }
  
  RendererLayer::~RendererLayer()
  {

  }
  
  void RendererLayer::OnAttach()
  {

  }
  
  void RendererLayer::OnDetach()
  {

  }
  
  void RendererLayer::OnUpdate(TimeStep ts)
  {

    
  }
  
  void RendererLayer::OnEvent(Event& event)
  {

  }
  
  void RendererLayer::OnImGuiRender()
  {
    
  }

  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
} // namespace Kreator
