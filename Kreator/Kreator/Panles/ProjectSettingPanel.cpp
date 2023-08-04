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
    if (UI::PropertyGridHeader("General"))
    {
      UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
  }
  
  void ProjectSettingsPanel::RenderLogSettings()
  {
 
  }
} // namespace Kreator
