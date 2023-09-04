//
//  ProjectSettingPanel.cpp
//  Kreator
//
//  Created by Ashish . on 04/08/23.
//

#include "ProjectSettingPanel.hpp"

namespace Kreator
{
  static bool s_serializeProject = false;
  
  void ProjectSettingsPanel::OnImGuiRender(bool& isOpen)
  {
    if (m_project == nullptr)
    {
      isOpen = false;
      return;
    }
    
    ImGui::Begin("Project Settings", &isOpen);
    RenderGeneralSettings();
#ifdef IK_ENABLE_LOG
    RenderLogSettings();
#endif
    ImGui::End();
    
    if (s_serializeProject)
    {
      ProjectSerializer serializer(m_project);
      serializer.Serialize(m_project->GetConfig().projectDirectory + "/" + m_project->GetConfig().projectFileName);
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
    if (Kreator_UI::PropertyGridHeader("General"))
    {
      Kreator_UI::BeginPropertyGrid(2, 2);
      UI::ScopedStyle frameRound(ImGuiStyleVar_FrameRounding, 20);
      
      {
        UI::ScopedDisable disable;
        Kreator_UI::Property("Name", m_project->GetConfig().name);
        Kreator_UI::Property("Asset Directory", m_project->GetConfig().assetDirectory);
        Kreator_UI::Property("Asset Registry Path", m_project->GetConfig().assetRegistryPath);
      }
      
      if (Kreator_UI::Property("Auto-save active scene", m_project->GetConfig().enableAutoSave))
      {
        s_serializeProject = true;
      }
      
      // 1 minute to 2 hours allowed range for auto-save.  Somewhat arbitrary...
      if (Kreator_UI::PropertySlider("Auto save interval (seconds)", m_project->GetConfig().autoSaveIntervalSeconds, 60, 7200))
      {
        s_serializeProject = true;
      }
      
      if (Kreator_UI::PropertyAssetReference<IKan::Scene>("Startup Scene", m_defaultScene))
      {
        const auto& metadata = AssetManager::GetMetadata(m_defaultScene);
        if (metadata.IsValid())
        {
          m_project->GetConfig().startScene = metadata.filePath.string();
          s_serializeProject = true;
        }
      }
      
      Kreator_UI::EndPropertyGrid();
      Kreator_UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
  }
  
  void ProjectSettingsPanel::RenderLogSettings()
  {
    ImGui::PushID("LogSettings");
    if (Kreator_UI::PropertyGridHeader("Log", false))
    {
      Kreator_UI::BeginPropertyGrid(3, 1);
      auto& tags = Log::EnabledTags();
      for (auto& [name, details] : tags)
      {
        // Don't show untagged log
        if (!name.empty())
        {
          ImGui::PushID(name.c_str());
          
          Kreator_UI::Property(name.c_str(), details.isEnabled);
          
          const char* levelStrings[] = { "Trace", "Info", "Warn", "Error", "Fatal"};
          int currentLevel = (int)details.levelFilter;
          if (Kreator_UI::PropertyDropdownNoLabel("LevelFilter", levelStrings, 5, &currentLevel))
          {
            details.levelFilter = (Log::Level)currentLevel;
          }
          ImGui::PopID();
        }
      }
      Kreator_UI::EndPropertyGrid();
      ImGui::TreePop();
    }
    ImGui::PopID();
  }
} // namespace Kreator
