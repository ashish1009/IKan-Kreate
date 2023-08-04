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
    
  void ProjectSettingsPanel::OnImguiRender(bool& isOpen)
  {
    if (m_project == nullptr)
    {
      isOpen = false;
      return;
    }
    
    ImGui::Begin("Project Settings", &isOpen);
    RenderGeneralSettings();
    RenderLogSettings();
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
      Kreator_UI::BeginPropertyGrid();
      
      {
        UI::ScopedDisable disable;
        Kreator_UI::Property("Name", m_project->GetConfig().name);
        Kreator_UI::Property("Asset Directory", m_project->GetConfig().assetDirectory);
        Kreator_UI::Property("Asset Registry Path", m_project->GetConfig().assetRegistryPath);
      }

      Kreator_UI::EndPropertyGrid();
      Kreator_UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
  }
  
  void ProjectSettingsPanel::RenderLogSettings()
  {
 
  }
} // namespace Kreator
