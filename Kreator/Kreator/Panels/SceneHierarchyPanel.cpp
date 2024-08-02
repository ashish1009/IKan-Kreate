//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 02/08/24.
//

#include "SceneHierarchyPanel.hpp"

#include "Layers/RendererLayer.hpp"

namespace Kreator
{
  void SceneHierarchyPanel::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("SceneHierarchyPanel", "Initializing the Scene Hierarchy Panel");
    s_pencilIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
    s_reloadIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Rotate.png");
    s_closeEyeIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/CloseEye.png");
    s_EyeIcon = TextureFactory::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Eye.png");
  }
  void SceneHierarchyPanel::Shutdown()
  {
    IK_PROFILE();
    s_pencilIcon.reset();
    s_plusIcon.reset();
    s_gearIcon.reset();
    s_reloadIcon.reset();
    IK_LOG_INFO("SceneHierarchyPanel", "Shutting down the Scene Hierarchy Panel");
  }
  
  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  : m_context(context), m_isWindow(isWindow)
  {
    IK_PROFILE();
    IK_LOG_INFO("Scene Hierarchy Panel", "Creating Scene Hierarchy Panel instance");
  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    IK_PROFILE();
  }
  
  void SceneHierarchyPanel::OnProjectChanged(const Ref<Project>& project)
  {
    IK_PROFILE();
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("SceneHierarchyPanel::OnImGuiRender");
    ImGui::Begin("Scene Hierarchy", &isOpen);
    ImGui::End();
    
    ImGui::Begin("Properties");
    ImGui::End();
  }
} //  namesapce Kreator
