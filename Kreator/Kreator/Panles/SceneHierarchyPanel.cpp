//
//  SceneHierarchyPanel.cpp
//  Kreator
//
//  Created by Ashish . on 05/08/23.
//

#include "SceneHierarchyPanel.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  void SceneHierarchyPanel::Initialize()
  {
    s_pencilIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Gear.png");
  }
  void SceneHierarchyPanel::Shutdown()
  {
    s_pencilIcon.reset();
    s_plusIcon.reset();
    s_gearIcon.reset();
  }

  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  : m_context(context), m_isWindow(isWindow)
  {

  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {
    m_context = scene;
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    if (m_isWindow)
    {
      UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("Scene Hierarchy", &isOpen);
    }
    
    if (m_context)
    {
      ImGui::End();
      
      {
        UI::ScopedStyle windowPadding(ImGuiStyleVar_WindowPadding, ImVec2(2.0, 4.0f));
        ImGui::Begin("Properties");
      }
    }
    
    if (m_isWindow)
    {
      ImGui::End();
    }
  }
} // namespace Kreator
