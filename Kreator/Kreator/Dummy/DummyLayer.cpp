//
//  DummyLayer.cpp
//  Kreator
//
//  Created by Ashish . on 08/10/23.
//

#include "DummyLayer.hpp"

namespace Kreator
{
  DummyLayer* DummyLayer::s_instance = nullptr;
  
  DummyLayer& DummyLayer::Get()
  {
    return *s_instance;
  }
  
  DummyLayer::DummyLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "DummyLayer instance already created");
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
  }
  
  DummyLayer::~DummyLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void DummyLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
  }
  
  void DummyLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
  }
  
  void DummyLayer::OnUpdate(TimeStep ts)
  {
    
  }
  
  void DummyLayer::OnEvent(Event& event)
  {
  }
  
  void DummyLayer::OnImGuiRender()
  {
    
  }
  
  void DummyLayer::OpenScene(const std::string& filepath)
  {
    
  }
  
  std::filesystem::path DummyLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }
  
} // namespace Kreator
