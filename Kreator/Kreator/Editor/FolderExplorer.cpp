//
//  FolderExplorer.cpp
//  Kreator
//
//  Created by Ashish . on 03/08/23.
//

#include "FolderExplorer.hpp"
#include "RendererLayer.hpp"

namespace Kreator
{
  enum class PopupType
  {
    Select, Open, Save
  };
  
  struct Data
  {
    bool popup = false;
    bool* lastPopupFlag;

    GUI_InputBuffer<256> openPathBuffer;
    GUI_InputBuffer<256> saveFileBuffer;

    PopupType popupType;
    std::filesystem::path currentPath;
    std::filesystem::path selectedFilePath = "";
    Ref<Image> shadowwTexture;
    Ref<Image> backButton;
    Ref<Image> folderIcon, fileTex;
  };
  Scope<Data> s_fileExplorerData;
  
  void FolderExplorer::Initialize()
  {
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowwTexture = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/ShadowLineTop.png");
    s_fileExplorerData->backButton = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Back.png");
    s_fileExplorerData->folderIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Folder.png");
    s_fileExplorerData->fileTex = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/File.png");
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    return "";
  }
  
  void FolderExplorer::SelectPopup(const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    s_fileExplorerData->popupType = PopupType::Select;
    PopupImpl(basePath, lastPopupFlag);
  }
  
  void FolderExplorer::OpenPopup(const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    s_fileExplorerData->popupType = PopupType::Open;
    PopupImpl(basePath, lastPopupFlag);
  }
  
  void FolderExplorer::SavePopup(const std::filesystem::path& basePath)
  {
    s_fileExplorerData->popupType = PopupType::Save;
    PopupImpl(basePath, nullptr);
  }
  
  void FolderExplorer::PopupImpl(const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    IK_ASSERT(Utils::FileSystem::Exists(basePath));
    
    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = Utils::FileSystem::IKanAbsolute(basePath);
    s_fileExplorerData->selectedFilePath = "";
    
    s_fileExplorerData->openPathBuffer.Memset(0);
    s_fileExplorerData->saveFileBuffer.Memset(0);
  }
} // namespace Kreator
