//
//  EditorPanelManager.cpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#include "EditorPanelManager.hpp"

namespace IKan
{
  PanelManager::~PanelManager()
  {
    m_panels.clear();
  }
  
  void PanelManager::OnImGuiRender() {
    for (auto& [id, panelData] : m_panels)
    {
      if (panelData.isOpen)
      {
        panelData.panel->OnImGuiRender(panelData.isOpen);
      }
    }
  }
  
  void PanelManager::OnEvent(Event& e)
  {
    for (auto& [id, panelData] : m_panels)
    {
      panelData.panel->OnEvent(e);
    }
  }
  
  void PanelManager::OnProjectChanged(const Ref<Project>& project)
  {
    for (auto& [id, panelData] : m_panels)
    {
      panelData.panel->OnProjectChanged(project);
    }
  }
  
  void PanelManager::SetSceneContext(const Ref<Scene>& context)
  {
    for (auto& [id, panelData] : m_panels)
    {
      panelData.panel->SetSceneContext(context);
    }
  }
  
  void PanelManager::RemovePanel(const char* strID)
  {
    uint32_t id = Hash::GenerateFNV(strID);
    if (m_panels.find(id) == m_panels.end())
    {
      IK_LOG_ERROR(LogModule::PanelManager, "Couldn't find panel with id '{0}'", strID);
      return;
    }
    m_panels.erase(id);
  }
  
  std::unordered_map<PanelID , PanelData>& PanelManager::GetPanels()
  {
    return m_panels;
  }
  const std::unordered_map<PanelID, PanelData>& PanelManager::GetPanels() const
  {
    return m_panels;
  }
  
} // namespace IKan
