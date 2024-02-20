//
//  ProjectSettingPanel.cpp
//  Kreator
//
//  Created by Ashish . on 20/02/24.
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
    UI::ScopedColor header(ImGuiCol_Header, UI::Color::BackgroundPopup);
    if (UI::PropertyGridHeader("General", true, 4))
    {
      UI::BeginPropertyGrid(2, 2);

      UI::EndPropertyGrid();
      UI::PropertyGridHeaderEnd();
    }
    ImGui::PopID();
  }
} // namespace Kreator
