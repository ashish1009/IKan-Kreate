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

    std::string title;
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
    IK_PROFILE();
    IK_LOG_TRACE("FolderExplorer", "Initialising the FolderExplorer textures");
    s_fileExplorerData = CreateScope<Data>();
    s_fileExplorerData->shadowwTexture = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/ShadowLineTop.png");
    s_fileExplorerData->backButton = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Back.png");
    s_fileExplorerData->folderIcon = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/Folder.png");
    s_fileExplorerData->fileTex = Image::Create(RendererLayer::Get().GetClientResorucePath() / "Textures/Icons/File.png");
  }
  
  void FolderExplorer::Shutdown()
  {
    s_fileExplorerData.reset();
    IK_LOG_WARN("FolderExplorer", "Shutting Down the FolderExplorer textures");
  }
  
  std::filesystem::path FolderExplorer::Explore()
  {
    std::filesystem::path returnPath = "";
    s_fileExplorerData->openPathBuffer.MemCpy(s_fileExplorerData->currentPath.c_str(), 0, s_fileExplorerData->currentPath.string().size());

    if (s_fileExplorerData->popup)
    {
      ImGui::OpenPopup(s_fileExplorerData->title.c_str());
      s_fileExplorerData->popup = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2{ 700, 325 });
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
    
    if (ImGui::BeginPopupModal(s_fileExplorerData->title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove ))
    {
      // Render Icon Buttons
      auto browserButton = [](const char* labelId, const Ref<Image>& icon)
      {
        const ImU32 buttonCol = UI::Theme::Color::BackgroundDark;
        const ImU32 buttonColP = UI::ColorWithMultipliedValue(UI::Theme::Color::BackgroundDark, 0.8f);
        UI::ScopedColorStack buttonColors(ImGuiCol_Button, buttonCol,
                                          ImGuiCol_ButtonHovered, buttonCol,
                                          ImGuiCol_ButtonActive, buttonColP);
        
        const float iconSize = 20.0f;
        const float iconPadding = 3.0f;
        const bool clicked = ImGui::Button(labelId, ImVec2(iconSize, iconSize));
        UI::DrawButtonImage(icon, Kreator_UI::Color::TextDarker,
                            UI::ColorWithMultipliedValue(Kreator_UI::Color::TextDarker, 1.2f),
                            UI::ColorWithMultipliedValue(Kreator_UI::Color::TextDarker, 0.8f),
                            UI::RectExpanded(UI::GetItemRect(), -iconPadding, -iconPadding));
        
        return clicked;
      };
      
      if (browserButton("##back", s_fileExplorerData->backButton))
      {
        if (s_fileExplorerData->currentPath != "/")
        {
          s_fileExplorerData->currentPath = s_fileExplorerData->currentPath.parent_path();
          s_fileExplorerData->openPathBuffer.Memset(0);
        }
      }
      
      ImGui::SameLine();
      
      float addressBarWidth = s_fileExplorerData->popupType == PopupType::Save ? 330 : 550;
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Theme::Color::Muted);
        ImGui::SetNextItemWidth(addressBarWidth);
        ImGui::InputTextWithHint("##new_project_location", "Project Location",
                                 s_fileExplorerData->openPathBuffer.Data(),
                                 s_fileExplorerData->openPathBuffer.Size(), ImGuiInputTextFlags_ReadOnly);
      }
      if (s_fileExplorerData->popupType == PopupType::Save)
      {
        UI::ScopedColor muted(ImGuiCol_Text, UI::Theme::Color::TextBrighter);
        ImGui::SameLine();
        ImGui::InputTextWithHint("##new_project_name", "File Name",
                                 s_fileExplorerData->saveFileBuffer.Data(),
                                 s_fileExplorerData->saveFileBuffer.Size());
      }
      
      ImGui::SameLine();
      std::string buttonTitle = "";
      switch(s_fileExplorerData->popupType)
      {
        case PopupType::Select : buttonTitle = "Select";  break;
        case PopupType::Open   : buttonTitle = "Open";    break;
        case PopupType::Save :   buttonTitle = "Save";    break;
        default:
          IK_ASSERT(false);
      }
      
      if (UI::DrawRoundButton(buttonTitle.c_str(), Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 10) or
          ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter))
      {
        bool isValid = true;
        switch(s_fileExplorerData->popupType)
        {
          case PopupType::Select :
          {
            returnPath = s_fileExplorerData->currentPath;
            break;
          }
          case PopupType::Open :
          {
            returnPath = s_fileExplorerData->selectedFilePath;
            
            // For Open file there must be some file selected to close the popup
            if (returnPath == "")
            {
              isValid = false;
            }
            break;
          }
          case PopupType::Save :
          {
            returnPath = s_fileExplorerData->currentPath / std::string(s_fileExplorerData->saveFileBuffer);
            break;
          }
          default:
            IK_ASSERT(false);
        }
        
        if (isValid)
        {
          if (s_fileExplorerData->popupType == PopupType::Select)
          {
            if (s_fileExplorerData->lastPopupFlag)
            {
              *s_fileExplorerData->lastPopupFlag = true;
            }
          }
          ImGui::CloseCurrentPopup();
        }
      }
      
      ImGui::SameLine();
      if (UI::DrawRoundButton("Close", Kreator_UI::ColorVec3FromU32(Kreator_UI::Color::NiceBlue), 10) or
          ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape))
        
      {
        ImGui::CloseCurrentPopup();
        if (s_fileExplorerData->lastPopupFlag)
        {
          *s_fileExplorerData->lastPopupFlag = true;
        }
      }
      
      ImGui::Separator();
      
      UI::PushID();
      if (ImGui::BeginTable(UI::GenerateID(), 1 /* Num Columns */, ImGuiTableFlags_SizingFixedFit, ImVec2(0.0f, 0.0f)))
      {
        UI::ScopedColor childBg(ImGuiCol_ChildBg, IM_COL32(43, 53, 67, 255));
        ImGui::TableSetupColumn("Open Folder", ImGuiTableColumnFlags_WidthStretch, 0);
        ImGui::TableNextRow();
        
        // Content Outliner
        ImGui::TableSetColumnIndex(0);
        ImGui::BeginChild("##Open_Folder", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - 104));
        {
          {
            UI::ScopedStyle spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
            UI::ScopedStyle framePadding(ImGuiStyleVar_FramePadding, ImVec2(5.0f, 5.0f));
            
            for (auto entry : std::filesystem::directory_iterator(s_fileExplorerData->currentPath))
            {
              if (entry.path().filename() == ".DS_Store")
              {
                continue;
              }
              
              auto directoryName = std::filesystem::relative(entry.path(), s_fileExplorerData->currentPath);
              std::string name = directoryName.filename().string();
              std::string id = name + "_TreeNode";
              
              // ImGui item height hack
              auto* window = ImGui::GetCurrentWindow();
              window->DC.CurrLineSize.y = 20.0f;
              window->DC.CurrLineTextBaseOffset = 3.0f;
              
              ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_FramePadding
              | ((s_fileExplorerData->selectedFilePath == entry) ? ImGuiTreeNodeFlags_Selected : 0);
              
              // Tree Node
              auto tex = entry.is_directory() ? s_fileExplorerData->folderIcon : s_fileExplorerData->fileTex;
              bool open = UI::TreeNode(id, name, flags, tex);
              if (ImGui::IsItemClicked())
              {
                if (entry.is_directory())
                {
                  s_fileExplorerData->currentPath = entry;
                  if (s_fileExplorerData->popupType == PopupType::Open)
                  {
                    s_fileExplorerData->selectedFilePath = "";
                  }
                }
                else
                {
                  if (s_fileExplorerData->popupType == PopupType::Open)
                  {
                    s_fileExplorerData->selectedFilePath = entry;
                  }
                }
              }
              
              // Fixing slight overlap
              UI::ShiftCursorY(3.0f);
              
              // If item clicked
              if (open)
              {
                ImGui::TreePop();
              }
            }
          }
        }
        ImGui::EndChild(); // folders_fileExplorerData->common
        ImGui::EndTable();
      }
      UI::PopID();
      ImGui::EndPopup();
    }
    ImGui::PopStyleVar(2);
    return returnPath;
  }
  
  void FolderExplorer::SelectPopup(const std::string& title, const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    s_fileExplorerData->title = title;
    s_fileExplorerData->popupType = PopupType::Select;
    PopupImpl(basePath, lastPopupFlag);
  }
  
  void FolderExplorer::OpenPopup(const std::string& title, const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    s_fileExplorerData->title = title;
    s_fileExplorerData->popupType = PopupType::Open;
    PopupImpl(basePath, lastPopupFlag);
  }
  
  void FolderExplorer::SavePopup(const std::string& title, const std::filesystem::path& basePath)
  {
    s_fileExplorerData->title = title;
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
