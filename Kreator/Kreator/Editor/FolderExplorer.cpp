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
    Select, Open
  };

  struct Data
  {
    bool popup = false;
    bool* lastPopupFlag;
    char* openPathBuffer;
    PopupType popupType;
    std::filesystem::path currentPath;
    std::filesystem::path selectedFilePath = "";
    Ref<Image> shadowwTexture;
    Ref<Image> backButton;
    Ref<Image> folderIcon, fileTex;

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
    s_fileExplorerData->backButton = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/Back.png");
    s_fileExplorerData->folderIcon = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/Folder.png");
    s_fileExplorerData->fileTex = Image::Create(RendererLayer::GetClientFilePath() / "Resources/Textures/Icons/File.png");
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 10));
    
    if (ImGui::BeginPopupModal("Folder Explorer", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                               ImGuiWindowFlags_NoTitleBar))
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
          memset(s_fileExplorerData->openPathBuffer, 0, MAX_PATH_LENGTH);
        }
      }
      
      ImGui::SameLine();
      
      float addressBarWidth = 550;
      ImGui::SetNextItemWidth(addressBarWidth);
      ImGui::InputTextWithHint("##new_project_location", "Project Location",
                               s_fileExplorerData->openPathBuffer, MAX_PATH_LENGTH, ImGuiInputTextFlags_ReadOnly);
      
      ImGui::SameLine();
      std::string buttonTitle = "";
      switch(s_fileExplorerData->popupType)
      {
        case PopupType::Select : buttonTitle = "Select";  break;
        case PopupType::Open   : buttonTitle = "Open";    break;
        default:
          IK_ASSERT(false);
      }
      
      if ((ImGui::Button(buttonTitle.c_str())) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Enter)))
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
      if ((ImGui::Button("Close")) or (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_Escape)))
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
        ImGui::BeginChild("##Open_Folder", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - 85));
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

  void FolderExplorer::PopupImpl(const std::filesystem::path& basePath, bool* lastPopupFlag)
  {
    IK_ASSERT(Utils::FileSystem::Exists(basePath));

    s_fileExplorerData->popup = true;
    s_fileExplorerData->lastPopupFlag = lastPopupFlag;
    s_fileExplorerData->currentPath = Utils::FileSystem::IKanAbsolute(basePath);
    s_fileExplorerData->selectedFilePath = "";
  }
} // namespace Kreator
