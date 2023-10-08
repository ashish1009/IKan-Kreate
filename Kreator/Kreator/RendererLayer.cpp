//
//  RendererLayer.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/23.
//

#include "RendererLayer.hpp"
#include "FolderExplorer.hpp"
#include "KreatorConsolePanel.hpp"
#include "ProjectSettingPanel.hpp"
#include "AssetPanel.hpp"
#include "ContentBrowserPanel.hpp"
#include "DefaultAssetViewer.hpp"
#include "SceneHierarchyPanel.hpp"
#include "DefaultMeshes.hpp"

extern std::string IKanVersion;

namespace Kreator
{
  RendererLayer* RendererLayer::s_instance = nullptr;
  
  RendererLayer& RendererLayer::Get()
  {
    return *s_instance;
  }

  RendererLayer::RendererLayer(Ref<UserPreferences> userPreference, const std::filesystem::path& clientResourcePath)
  : Layer("Kreator Renderer"), m_clientResourcePath(clientResourcePath)
  {
    IK_PROFILE();
    IK_ASSERT(!s_instance, "RendererLayer instance already created");
    // Copy the single instance of application
    s_instance = this;
    
    IK_LOG_TRACE("Kreator Layer", "Creating Kreator Renderer Layer instance");
  }
  
  RendererLayer::~RendererLayer()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Destroying Kreator Renderer Layer instance");
  }
  
  void RendererLayer::OnAttach()
  {
    IK_PROFILE();
    IK_LOG_TRACE("Kreator Layer", "Attaching Kreator Renderer Layer to application");
  }
  
  void RendererLayer::OnDetach()
  {
    IK_PROFILE();
    IK_LOG_WARN("Kreator Layer", "Detaching Kreator Renderer Layer from application");
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

  void RendererLayer::OpenScene(const std::string& filepath)
  {
    
  }
  
  std::filesystem::path RendererLayer::GetClientResorucePath() const
  {
    return m_clientResourcePath;
  }

} // namespace Kreator
