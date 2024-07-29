//
//  ProjectSettingPanel.cpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#include "ProjectSettingPanel.hpp"

namespace Kreator
{
  static bool s_serializeProject = false;
  
  void ProjectSettingsPanel::OnImGuiRender(bool& isOpen)
  {
    IK_PERFORMANCE("ProjectSettingsPanel::OnImGuiRender");
    if (m_project == nullptr)
    {
      isOpen = false;
      return;
    }
    
    ImGui::Begin("Project Settings", &isOpen);
    RenderGeneralSettings();
    ImGui::End();
    
    if (s_serializeProject)
    {
      ProjectSerializer serializer(m_project);
      serializer.Serialize(m_project->GetConfig().projectDirectory / m_project->GetConfig().projectFileName);
      s_serializeProject = false;
    }
  }
  
  void ProjectSettingsPanel::OnProjectChanged(const Ref<Project>& project)
  {
    m_project = project;
    m_defaultScene = AssetManager::GetAssetHandleFromFilePath(project->GetConfig().startScene);
  }
  
  void ProjectSettingsPanel::RenderGeneralSettings()
  {
    ImGui::PushID("GeneralSettings");
    UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 5);
    
    if (UI::PropertyGridHeader("General", true, 4))
    {
      UI::BeginPropertyGrid(2, 1, 5);
      
      {
        UI::ScopedDisable disable;
        UI::Property("Name", m_project->GetConfig().name);
        UI::Property("Asset Directory", m_project->GetConfig().assetDirectory.string());
        UI::Property("Asset Registry Path", m_project->GetConfig().assetRegistryPath.string());
      }
      
      bool enableAutoSave = m_project->GetConfig().enableAutoSave;
      if (UI::Property("Auto-save active scene", enableAutoSave))
      {
        m_project->UpdateAutoSaveScene(enableAutoSave);
        s_serializeProject = true;
      }
      
      // 1 minute to 2 hours allowed range for auto-save.  Somewhat arbitrary...
      int32_t autoSaveIntervalSeconds = m_project->GetConfig().autoSaveIntervalSeconds;
      if (UI::PropertySlider("Auto save interval (seconds)", autoSaveIntervalSeconds, 60, 7200))
      {
        m_project->UpdateAutoSaveSceneTimeInterval(autoSaveIntervalSeconds);
        s_serializeProject = true;
      }
      
      std::filesystem::path startScene = m_project->GetConfig().startScene;
      if (UI::PropertyAssetReference<IKan::Scene>("Startup Scene", m_defaultScene))
      {
        const auto& metadata = AssetManager::GetMetadata(m_defaultScene);
        {
          m_project->UpdateStartupScene(metadata.filePath);
          s_serializeProject = true;
        }
      }
      
      UI::EndPropertyGrid();
      UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
  }
} // namespace Kreator
