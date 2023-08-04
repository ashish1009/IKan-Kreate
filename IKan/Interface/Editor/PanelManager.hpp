//
//  PanelManager.hpp
//  IKan
//
//  Created by Ashish . on 04/08/23.
//

#pragma once

#include "Editor/EditorPanel.hpp"

namespace IKan
{
  using PanelID = uint32_t;
  
  /// Stores the Panel data of each editor panels
  struct PanelData
  {
    const char* ID = "";
    const char* name = "";
    Ref<EditorPanel> panel;
    bool isOpen = false;
  };
  
  /// This class manages all the editor panels
  class PanelManager
  {
  public:
    /// Defatuls constructor
    PanelManager() = default;
    /// Destrcutor of Panel manager
    ~PanelManager();
    
    // Template API to Add and Use Panel -----------------------------------------------------------------------------
    template<typename TPanel>
    Ref<TPanel> AddPanel(const PanelData& panelData)
    {
      IK_LOG_TRACE(LogModule::PanelManager, "Adding {0} Panel in Panel List (ID : {1})", panelData.name, panelData.ID);
      static_assert(std::is_base_of<EditorPanel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");
      uint32_t id = Hash::GenerateFNV(panelData.ID);
      if (m_panels.find(id) != m_panels.end())
      {
        IK_LOG_ERROR(LogModule::PanelManager, "A panel with the ID '{0}' has already been added.", panelData.ID);
        return nullptr;
      }
      m_panels[id] = panelData;
      return std::static_pointer_cast<TPanel>(panelData.panel);
    }
    
    template<typename TPanel, typename... TArgs>
    Ref<TPanel> AddPanel(const char* strID, bool isOpenByDefault, TArgs&&... args)
    {
      return AddPanel<TPanel>(PanelData{strID, strID, CreateRef<TPanel>(std::forward<TArgs>(args)...), isOpenByDefault });
    }
    
    template<typename TPanel, typename... TArgs>
    Ref<TPanel> AddPanel(const char* strID, const char* displayName, bool isOpenByDefault, TArgs&&... args)
    {
      return AddPanel<TPanel>(PanelData{strID, displayName, CreateRef<TPanel>(std::forward<TArgs>(args)...), isOpenByDefault });
    }
    template<typename TPanel>
    Ref<TPanel> GetPanel(const char* strID)
    {
      static_assert(std::is_base_of<EditorPanel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from EditorPanel");
      uint32_t id = Hash::GenerateFNV(strID);
      if (m_panels.find(id) == m_panels.end())
      {
        IK_LOG_ERROR(LogModule::PanelManager, "Couldn't find panel with id '{0}'", strID);
        return nullptr;
      }
      return std::static_pointer_cast<TPanel>(m_panels.at(id).panel);
    }
    
  private:
    std::unordered_map<PanelID, PanelData> m_panels;
  };
} // namespace IKan
