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
#define MAX_PATH_LENGTH 255

  using namespace IKan;
  
  enum class PopupType
  {
    Open, Select, Save
  };

  struct Data
  {
    bool popup = false;
    bool* lastPopupFlag;
    char* openPathBuffer;
    PopupType popupType;
    std::filesystem::path currentPath;
    Ref<Image> shadowwTexture;
    
    Data()
    {
      openPathBuffer = iknew char[MAX_PATH_LENGTH];
    }
    
    ~Data()
    {
      ikdelete [] openPathBuffer;
    }

  };
  Scope<Data> s_fileExplorerData;

  void FolderExplorer::Initialize()
  {
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowwTexture = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/ShadowLineTop.png");
  }
  
  void FolderExplorer::Shutdown()
  {
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
    s_fileExplorerData.reset();
  }

  std::filesystem::path FolderExplorer::Explore()
  {
    std::filesystem::path returnPath = "";
    
    memcpy(s_fileExplorerData->openPathBuffer, s_fileExplorerData->currentPath.c_str(), s_fileExplorerData->currentPath.string().size());

    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup("Folder Explorer");
      s_fileExplorerData->popup = false;
    }
    
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 325 });
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));

    if (ImGui::BeginPopupModal("Folder Explorer", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove
                               | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
    {
      const float topBarHeight = 40;
      auto* drawList = ImGui::GetWindowDrawList();
      
      // Draw the adress Bar rectangle ---------------------------------------------------
      {
        const ImVec2 addresBarMin = ImGui::GetCursorScreenPos();
        const ImVec2 addresBarMax =
        {
          addresBarMin.x + ImGui::GetWindowWidth(),
          addresBarMin.y + topBarHeight
        };
        
        drawList->AddRectFilled(addresBarMin, addresBarMax, UI::Theme::Color::BackgroundPopup);
      }
      
      // Draw the Main Area Bar rectangle -------------------------------------------------
      {
        const ImVec2 mainAreaMin =
        {
          ImGui::GetCursorScreenPos().x,
          ImGui::GetCursorScreenPos().y + topBarHeight
        };
        const ImVec2 mainAreaMax =
        {
          mainAreaMin.x + ImGui::GetWindowWidth(),
          mainAreaMin.y + 700 - topBarHeight
        };
        
        drawList->AddRectFilled(mainAreaMin, mainAreaMax, UI::Theme::Color::BackgroundPopup);
      }
      
      {
        UI::ScopedStyle windowPadding (ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
//        UI::ScopedColor frameBg(ImGuiCol_FrameBg, UI::Theme::Color::BackgroundPopup);

        UI::ShiftCursorX(ImGui::GetCurrentWindow()->WindowPadding.x);
        UI::ShiftCursorY(ImGui::GetCurrentWindow()->WindowPadding.y);

        float addressBarWidth = s_fileExplorerData->popupType == PopupType::Save ? 330 : 550;
        ImGui::SetNextItemWidth(addressBarWidth);
        ImGui::InputTextWithHint("##new_project_location", "Project Location",
                                 s_fileExplorerData->openPathBuffer, MAX_PATH_LENGTH, ImGuiInputTextFlags_ReadOnly);
      }

      // Draw top shadow-----------------------------------------------------------------
      ImRect shadowRect =
      {
        {
          ImGui::GetCursorScreenPos().x,
          ImGui::GetCursorScreenPos().y + 2
        },
        { ImGui::GetCursorScreenPos().x + ImGui::GetWindowWidth() - 11,
          ImGui::GetCursorScreenPos().y + 20
        }
      };

      ImRect windowRect = UI::RectExpanded(shadowRect, 0.0f, 0.0f);
      ImGui::PushClipRect(windowRect.Min, windowRect.Max, false);
      UI::DrawShadowInner(s_fileExplorerData->shadowwTexture, 10.0f, windowRect, 1.0f, windowRect.GetWidth() / 8,
                          false, false, true, false);

      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(1);
 
    return returnPath;
  }
  
  void FolderExplorer::SelectPopup(bool* lastPopupFlag, const std::filesystem::path& basePath)
  {
    s_fileExplorerData->popupType = PopupType::Select;
    PopupImpl(lastPopupFlag, basePath);
  }

  void FolderExplorer::PopupImpl(bool* lastPopupFlag, const std::filesystem::path& basePath)
  {
    IK_ASSERT(Utils::FileSystem::Exists(basePath));

    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = basePath;
  }
} // namespace Kreator
