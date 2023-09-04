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
  SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context, bool isWindow)
  {

  }
  
  void SceneHierarchyPanel::SetSceneContext(const Ref<Scene>& scene)
  {

  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
    UI::ScopedStyle padding(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene Hierarchy", &isOpen);
    ImGui::End();
  }
} // namespace Kreator
