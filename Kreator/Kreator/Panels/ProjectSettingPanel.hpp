//
//  ProjectSettingPanel.hpp
//  Kreator
//
//  Created by Ashish . on 29/07/24.
//

#pragma once

namespace Kreator
{
  class ProjectSettingsPanel : public EditorPanel
  {
  public:
    /// This is the consturctor to create Project Setting Window instance
    ProjectSettingsPanel() = default;
    /// This is the Project Setting window destructor
    ~ProjectSettingsPanel() = default;
    
    /// @see Editor Panel
    virtual void OnImGuiRender(bool& isOpen) override;
    /// @see Editor Panel
    virtual void OnProjectChanged(const Ref<Project>& project) override;
    
  private:
    // Member Functions ----------------------------------------------------------------------------------------------
    /// This function Renders the General Setting
    void RenderGeneralSettings();
    
    // Member Variables ----------------------------------------------------------------------------------------------
    Ref<Project> m_project;
    AssetHandle m_defaultScene;
  };
} // namespace Kreator
