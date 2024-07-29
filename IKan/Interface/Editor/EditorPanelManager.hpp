//
//  EditorPanelManager.hpp
//  IKan
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

#include "Editor/EditorPanel.hpp"

namespace IKan
{
  using PanelID = uint32_t;
  
  /// This structure stores the Panel data of each editor panels
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
    
    /// This function removes the Panel from map
    /// - Parameter strID: string ID
    void RemovePanel(const char* strID);
    
    // Editor Panel APIs ---------------------------------------------------------------------------------------------
    /// This function renders the ImGui for all Panels
    void OnImGuiRender();
    /// This function handle Events for all Panels
    /// - Parameter e: event handle
    void OnEvent(Event& e);
    /// This function handles the project change for all the panels
    /// - Parameter project: New project
    void OnProjectChanged(const Ref<Project>& project);
    /// This function updats the scene in all panels;
    /// - Parameter context: scene context
    void SetSceneContext(const Ref<Scene>& context);
    
    // Getters ------------------------------------------------------------------------------------------------------
    /// This function returns the reference of Panels
    std::unordered_map<PanelID, PanelData>& GetPanels();
    /// This function returns the const reference of Panels
    const std::unordered_map<PanelID, PanelData>& GetPanels() const;
    
    DELETE_COPY_MOVE_CONSTRUCTORS(PanelManager);
    
  private:
    std::unordered_map<PanelID, PanelData> m_panels;
  };
} // namespace IKan
