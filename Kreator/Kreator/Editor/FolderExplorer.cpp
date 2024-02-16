//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 16/02/24.
//

#include "FolderExplorer.hpp"

namespace Kreator
{
  enum class PopupType
  {
    Invalid, Select, Open, Save
  };
  
  struct Data
  {
    bool popup {false};
    PopupType popupType {PopupType::Invalid};
  };
  static Scope<Data> s_fileExplorerData;

  void FolderExplorer::Initialize()
  {
    IK_PROFILE();
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_PROFILE();
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    std::filesystem::path returnPath {};
    
    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup("Kreator File Viewer");
      s_fileExplorerData->popup = false;
    }
    
    UI::SetNextWindowAtCenterWithSize({700, 350});

    if (ImGui::BeginPopupModal("Kreator File Viewer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ))
    {
      ImGui::EndPopup();
    } // Begin Popup

    return returnPath;
  }
  
  void FolderExplorer::Select()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Select;
  }
  
  void FolderExplorer::Open()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Open;
  }
  
  void FolderExplorer::Save()
  {
    s_fileExplorerData->popup = true;
    s_fileExplorerData->popupType = PopupType::Save;
  }
} // namespace Kreator
