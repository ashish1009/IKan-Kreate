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
    s_pencilIcon = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Icons/Pencil.png");
    s_plusIcon = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Icons/Plus.png");
    s_gearIcon = Image::Create(RendererLayer::GetClientResorucePath() / "Textures/Icons/Gear.png");
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
    m_selectionContext = {};
    if(m_context)
    {
      m_context->SetEntityDestroyedCallback([this](Entity entity) { OnExternalEntityDestroyed(entity); });
    }
  }
  
  void SceneHierarchyPanel::OnImGuiRender(bool& isOpen)
  {
  }
  
  void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
  {
    m_selectionContext = entity;
    
    if (m_selectionChangedCallback)
    {
      m_selectionChangedCallback(m_selectionContext);
    }
  }

  void SceneHierarchyPanel::SetSelectionChangedCallback(const std::function<void(Entity)>& func)
  {
    m_selectionChangedCallback = func;
  }
  void SceneHierarchyPanel::SetEntityDeletedCallback(const std::function<void(Entity)>& func)
  {
    m_entityDeletedCallback = func;
  }
  
  void SceneHierarchyPanel::OnExternalEntityDestroyed(Entity entity)
  {
    if (entity == m_selectionContext)
    {
      m_selectionContext = {};
    }
  }
} // namespace Kreator
